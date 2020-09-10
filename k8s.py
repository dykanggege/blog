#coding=utf-8
from kubernetes import client, config
import ssl
import sys
import codecs

KUBECONFIG_FILE = "/usr/local/tke/tfs-ns.kubeconfig"
NAMESPACE = "tfs-ns"
sys.stdout = codecs.getwriter("utf-8")(sys.stdout.detach())

class K8s_op():
    def __init__(self):
        config.kube_config.load_kube_config(config_file="/usr/local/tke/tfs-ns.kubeconfig")
        self.k8s_v1 = client.CoreV1Api()

    #列出所有namespace名
    def list_all_namespace(self):
        name_list = []
        for namespace in self.k8s_v1.list_namespace().items:
            name = namespace.metadata.name
            name_list.append(name)
        return name_list

    # 列出可操作的namespace信息
    def list_namespace(self, ns_name = NAMESPACE):
        for namespace in self.k8s_v1.list_namespace().items:
            name = namespace.metadata.name
            if name == ns_name:
                return namespace
        return -1

    # 列出可操作的worload名
    def list_workload(self, ns_name = NAMESPACE):
        workload_list = []
        for pod in self.k8s_v1.list_namespaced_pod(ns_name).items:
            workload_name = pod.metadata.owner_references[0].name
            if workload_name not in workload_list:
                workload_list.append(workload_name)
        return workload_list

    #按workload名列出pod name和ip
    def list_pod_workload(self, workload_name, ns_name = NAMESPACE):
        pod_list = []
        for pod in self.k8s_v1.list_namespaced_pod(ns_name).items:
            pod_name = pod.metadata.name
            pod_ip = pod.status.pod_ip
            name = pod.metadata.owner_references[0].name
            if name == workload_name:
                pod_list.append({"pod_name":pod_name, "pod_ip":pod_ip})
        return pod_list

    #按pod_name删除单个pod
    def del_pod(self, pod_name, ns_name = NAMESPACE):
        return self.k8s_v1.delete_namespaced_pod(name=pod_name, namespace=ns_name)

    #按pod_name更换镜像
    def updateImage_pod(self, pod_name, image, ns_name = NAMESPACE):
        old_resp = self.k8s_v1.read_namespaced_pod(namespace=ns_name, name=pod_name)
        old_resp.spec.containers[0].image = image
        new_resp = self.k8s_v1.patch_namespaced_pod(namespace=ns_name,name=pod_name,body=old_resp)



if __name__ == '__main__':
    tke = K8s_op()
    #列出所有namespace名
    # print(tke.list_all_namespace())
    #
    # 列出可操作的namespace信息
    # print(tke.list_namespace())
    #
    # 列出可操作的worload名
    #print(tke.list_workload())
    #
    #按workload名列出pod name和ip
    # print(tke.list_pod_workload("wb-trxd"))

    #按pod_name删除单个pod
    # print(tke.del_pod("wb-trxd-1"))

    #按pod_name更换镜像
    # updateImage_pod("wb-trxd-1", "mirrors.tencent.com/capdtke/tlinux2.2_general:latest")
