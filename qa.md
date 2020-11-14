#!/bin/bash
set -e

export VIP=$1
export REAL_IP=$2
export CA_DIR="/etc/kubernetes/ssl/$VIP/"
export ADMIN_CONFIG_DIR="/root/.kube/$VIP/"

if [ -d $CA_DIR ]; then
    echo "$CA_DIR exist, remove it"
    rm -r  $CA_DIR
fi

if [ -d $ADMIN_CONFIG_DIR ]; then
    echo "$ADMIN_CONFIG_DIR exist, remove it"
    rm -r $ADMIN_CONFIG_DIR
fi

if [ ! -d $CA_DIR ]; then
    echo "mkdir $CA_DIR"
    mkdir -p $CA_DIR
fi

if [ ! -d $ADMIN_CONFIG_DIR ]; then
    echo "mkdir $ADMIN_CONFIG_DIR"
    mkdir -p $ADMIN_CONFIG_DIR
fi

echo "GET CA"
scp root@$REAL_IP:/etc/kubernetes/ssl/ca.pem $CA_DIR/ca.crt
scp root@$REAL_IP:/etc/kubernetes/ssl/ca-key.pem $CA_DIR/ca.key
#scp root@$REAL_IP:/etc/kubernetes/ca.crt $CA_DIR
#scp root@$REAL_IP:/etc/kubernetes/ca.key $CA_DIR
echo "GET ADMIN_CONFIG"
scp root@$REAL_IP:/root/.kube/config $ADMIN_CONFIG_DIR

sed -i "s/127.0.0.1/$VIP/g" $ADMIN_CONFIG_DIR/config























#!/bin/bash
set -e

export NAMESPACE="tfs-ns"
export USERNAME="tfs-ns"
export ENDPOINT="https://9.136.124.195:60002"
export KUBECONFIG_FILE="${USERNAME}.kubeconfig" 
export GROUP="capdtke"
export CA_CRT="/etc/kubernetes/ca.crt"
export CA_KEY="/etc/kubernetes/ca.key"
export DAYS=1800
export CLUSTER_NAME="kubernetes"


# gen user cert and key file
openssl genrsa -out ${USERNAME}.key 2048
openssl req -new -key ${USERNAME}.key -out ${USERNAME}.csr -subj "/CN=${USERNAME}/O=${GROUP}"
openssl x509 -req -in ${USERNAME}.csr -CA ${CA_CRT} -CAkey ${CA_KEY} -CAcreateserial -out ${USERNAME}.crt -days ${DAYS}

kubectl config set-cluster ${CLUSTER_NAME} \
--kubeconfig=${KUBECONFIG_FILE} \
--server=${ENDPOINT} \
--certificate-authority=${CA_CRT}  \
--embed-certs=true

kubectl config set-credentials ${USERNAME} \
--client-certificate=${USERNAME}.crt  \
--client-key=${USERNAME}.key \
--embed-certs=true \
--kubeconfig=${KUBECONFIG_FILE}

kubectl config set-context ${USERNAME}-context \
--cluster=${CLUSTER_NAME} \
--namespace=${NAMESPACE} \
--user=${USERNAME} \
--kubeconfig=${KUBECONFIG_FILE}

kubectl config use-context ${USERNAME}-context \
--kubeconfig=${KUBECONFIG_FILE}

kubectl create namespace ${NAMESPACE} || true


cat <<EOF > ${USERNAME}-role.yaml
---
kind: Role
apiVersion: rbac.authorization.k8s.io/v1beta1
metadata:
  namespace: ${NAMESPACE}
  name: ${USERNAME}-role
rules:
- apiGroups: ["", "extensions", "apps", "apps.tkestack.io"]
  resources: ["*"]
  verbs: ["*"]

---
kind: RoleBinding
apiVersion: rbac.authorization.k8s.io/v1beta1
metadata:
  name: ${USERNAME}-rolebinding
  namespace: ${NAMESPACE} 
subjects:
- kind: User
  name: ${USERNAME} 
  apiGroup: ""
roleRef:
  kind: Role
  name: ${USERNAME}-role 
  apiGroup: ""

---
kind: ClusterRole
apiVersion: rbac.authorization.k8s.io/v1beta1
metadata:
  namespace: ${NAMESPACE}
  name: ${USERNAME}-clusterrole
rules:
- apiGroups: [""]
  resources: ["nodes", "namespaces"]
  verbs: ["get", "list", "watch"]

---
kind: ClusterRoleBinding
apiVersion: rbac.authorization.k8s.io/v1beta1
metadata:
  name: ${USERNAME}-clusterrolebinding
  namespace: ${NAMESPACE} 
subjects:
- kind: User
  name: ${USERNAME} 
  apiGroup: ""
roleRef:
  kind: ClusterRole
  name: ${USERNAME}-clusterrole 
  apiGroup: ""

EOF
kubectl create -f ${USERNAME}-role.yaml

echo "DONE"

