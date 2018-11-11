require "bundler"
Bundler.require

set :bind, "0.0.0.0"
set :server, "thin"
set :libvirt, Libvirt::open("qemu+unix:///system?socket=/var/run/libvirt/libvirt-sock")

def nilify
  yield
rescue Libvirt::RetrieveError
  nil
end

def active_vm_uuids
  settings
    .libvirt
    .list_all_domains
    .select { |d| d.state.first == Libvirt::Domain::RUNNING }
    .sort { |a, b| b.id <=> a.id }
    .collect(&:uuid)
end

def find_vm_by_uuid(uuid)
  settings.libvirt.lookup_domain_by_uuid(uuid)
end

def vm
  find_vm_by_uuid(params[:uuid])
end

def handle_vm_errors
  yield
rescue Libvirt::RetrieveError
  status 404
rescue Libvirt::Error
  status 405
end

get "/active" do
  active_vm_uuids.join(",")
end

get "/:uuid/shutdown" do
  handle_vm_errors do
    vm.shutdown
  end
end

get "/:uuid/destroy" do
  handle_vm_errors do
    vm.destroy
  end
end
