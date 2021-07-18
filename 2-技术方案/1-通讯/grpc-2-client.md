pb生成的client端应包含所有接口函数，底层使用NewClient时传入的Conn的接口

```
func (c *authClient) ListNamespace(ctx context.Context, in *Token, opts ...grpc.CallOption) (*NamespaceList, error) {
	out := new(NamespaceList)
    // 可以看出对调用命名规则是 package + server + function
	err := c.cc.Invoke(ctx, "/nimblescan.auth.Auth/ListNamespace", in, out, opts...)
	if err != nil {
		return nil, err
	}
	return out, nil
}
```

DialContext 函数巨长，拆开来看

# 参数
```
cc := &ClientConn{
    target:            target,
    csMgr:             &connectivityStateManager{},
    conns:             make(map[*addrConn]struct{}),
    dopts:             defaultDialOptions(),
    blockingpicker:    newPickerWrapper(),
    czData:            new(channelzData),
    firstResolveEvent: grpcsync.NewEvent(),
}
```
用defaultDialOptions作为默认配置，其他配置通过实现DialOption接口的函数传入
```
type DialOption interface {
	apply(*dialOptions)
}
```
在默认配置基础上添加用户配置
```
for _, opt := range opts {
    opt.apply(&cc.dopts)
}
```
在必要配置不多的场景下，可以New时传入必须配置，其他给默认配置，并且通过函数方式让用户改变

如果参数太多，还是给个default基础上给用户改，否则写的函数太多了

# 拦截器
网络收发包场景下最需要切面

```
	cc.retryThrottler.Store((*retryThrottler)(nil))
	cc.ctx, cc.cancel = context.WithCancel(context.Background())

	for _, opt := range opts {
		opt.apply(&cc.dopts)
	}

	chainUnaryClientInterceptors(cc)
	chainStreamClientInterceptors(cc)

	defer func() {
		if err != nil {
			cc.Close()
		}
	}()

	if channelz.IsOn() {
		if cc.dopts.channelzParentID != 0 {
			cc.channelzID = channelz.RegisterChannel(&channelzChannel{cc}, cc.dopts.channelzParentID, target)
			channelz.AddTraceEvent(logger, cc.channelzID, 0, &channelz.TraceEventDesc{
				Desc:     "Channel Created",
				Severity: channelz.CtInfo,
				Parent: &channelz.TraceEventDesc{
					Desc:     fmt.Sprintf("Nested Channel(id:%d) created", cc.channelzID),
					Severity: channelz.CtInfo,
				},
			})
		} else {
			cc.channelzID = channelz.RegisterChannel(&channelzChannel{cc}, 0, target)
			channelz.Info(logger, cc.channelzID, "Channel Created")
		}
		cc.csMgr.channelzID = cc.channelzID
	}

	if !cc.dopts.insecure {
		if cc.dopts.copts.TransportCredentials == nil && cc.dopts.copts.CredsBundle == nil {
			return nil, errNoTransportSecurity
		}
		if cc.dopts.copts.TransportCredentials != nil && cc.dopts.copts.CredsBundle != nil {
			return nil, errTransportCredsAndBundle
		}
	} else {
		if cc.dopts.copts.TransportCredentials != nil || cc.dopts.copts.CredsBundle != nil {
			return nil, errCredentialsConflict
		}
		for _, cd := range cc.dopts.copts.PerRPCCredentials {
			if cd.RequireTransportSecurity() {
				return nil, errTransportCredentialsMissing
			}
		}
	}

	if cc.dopts.defaultServiceConfigRawJSON != nil {
		scpr := parseServiceConfig(*cc.dopts.defaultServiceConfigRawJSON)
		if scpr.Err != nil {
			return nil, fmt.Errorf("%s: %v", invalidDefaultServiceConfigErrPrefix, scpr.Err)
		}
		cc.dopts.defaultServiceConfig, _ = scpr.Config.(*ServiceConfig)
	}
	cc.mkp = cc.dopts.copts.KeepaliveParams

	if cc.dopts.copts.UserAgent != "" {
		cc.dopts.copts.UserAgent += " " + grpcUA
	} else {
		cc.dopts.copts.UserAgent = grpcUA
	}

	if cc.dopts.timeout > 0 {
		var cancel context.CancelFunc
		ctx, cancel = context.WithTimeout(ctx, cc.dopts.timeout)
		defer cancel()
	}
	defer func() {
		select {
		case <-ctx.Done():
			switch {
			case ctx.Err() == err:
				conn = nil
			case err == nil || !cc.dopts.returnLastError:
				conn, err = nil, ctx.Err()
			default:
				conn, err = nil, fmt.Errorf("%v: %v", ctx.Err(), err)
			}
		default:
		}
	}()

	scSet := false
	if cc.dopts.scChan != nil {
		// Try to get an initial service config.
		select {
		case sc, ok := <-cc.dopts.scChan:
			if ok {
				cc.sc = &sc
				cc.safeConfigSelector.UpdateConfigSelector(&defaultConfigSelector{&sc})
				scSet = true
			}
		default:
		}
	}
	if cc.dopts.bs == nil {
		cc.dopts.bs = backoff.DefaultExponential
	}

	// Determine the resolver to use.
	cc.parsedTarget = grpcutil.ParseTarget(cc.target, cc.dopts.copts.Dialer != nil)
	channelz.Infof(logger, cc.channelzID, "parsed scheme: %q", cc.parsedTarget.Scheme)
	resolverBuilder := cc.getResolver(cc.parsedTarget.Scheme)
	if resolverBuilder == nil {
		// If resolver builder is still nil, the parsed target's scheme is
		// not registered. Fallback to default resolver and set Endpoint to
		// the original target.
		channelz.Infof(logger, cc.channelzID, "scheme %q not registered, fallback to default scheme", cc.parsedTarget.Scheme)
		cc.parsedTarget = resolver.Target{
			Scheme:   resolver.GetDefaultScheme(),
			Endpoint: target,
		}
		resolverBuilder = cc.getResolver(cc.parsedTarget.Scheme)
		if resolverBuilder == nil {
			return nil, fmt.Errorf("could not get resolver for default scheme: %q", cc.parsedTarget.Scheme)
		}
	}

	creds := cc.dopts.copts.TransportCredentials
	if creds != nil && creds.Info().ServerName != "" {
		cc.authority = creds.Info().ServerName
	} else if cc.dopts.insecure && cc.dopts.authority != "" {
		cc.authority = cc.dopts.authority
	} else if strings.HasPrefix(cc.target, "unix:") || strings.HasPrefix(cc.target, "unix-abstract:") {
		cc.authority = "localhost"
	} else if strings.HasPrefix(cc.parsedTarget.Endpoint, ":") {
		cc.authority = "localhost" + cc.parsedTarget.Endpoint
	} else {
		// Use endpoint from "scheme://authority/endpoint" as the default
		// authority for ClientConn.
		cc.authority = cc.parsedTarget.Endpoint
	}

	if cc.dopts.scChan != nil && !scSet {
		// Blocking wait for the initial service config.
		select {
		case sc, ok := <-cc.dopts.scChan:
			if ok {
				cc.sc = &sc
				cc.safeConfigSelector.UpdateConfigSelector(&defaultConfigSelector{&sc})
			}
		case <-ctx.Done():
			return nil, ctx.Err()
		}
	}
	if cc.dopts.scChan != nil {
		go cc.scWatcher()
	}

	var credsClone credentials.TransportCredentials
	if creds := cc.dopts.copts.TransportCredentials; creds != nil {
		credsClone = creds.Clone()
	}
	cc.balancerBuildOpts = balancer.BuildOptions{
		DialCreds:        credsClone,
		CredsBundle:      cc.dopts.copts.CredsBundle,
		Dialer:           cc.dopts.copts.Dialer,
		CustomUserAgent:  cc.dopts.copts.UserAgent,
		ChannelzParentID: cc.channelzID,
		Target:           cc.parsedTarget,
	}

	// Build the resolver.
	rWrapper, err := newCCResolverWrapper(cc, resolverBuilder)
	if err != nil {
		return nil, fmt.Errorf("failed to build resolver: %v", err)
	}
	cc.mu.Lock()
	cc.resolverWrapper = rWrapper
	cc.mu.Unlock()

	// A blocking dial blocks until the clientConn is ready.
	if cc.dopts.block {
		for {
			s := cc.GetState()
			if s == connectivity.Ready {
				break
			} else if cc.dopts.copts.FailOnNonTempDialError && s == connectivity.TransientFailure {
				if err = cc.connectionError(); err != nil {
					terr, ok := err.(interface {
						Temporary() bool
					})
					if ok && !terr.Temporary() {
						return nil, err
					}
				}
			}
			if !cc.WaitForStateChange(ctx, s) {
				// ctx got timeout or canceled.
				if err = cc.connectionError(); err != nil && cc.dopts.returnLastError {
					return nil, err
				}
				return nil, ctx.Err()
			}
		}
	}

	return cc, nil

```
