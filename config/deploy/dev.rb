set :deploy_to, "/services/create/"
if(branch = ENV['BRANCH'])
  set :branch, branch
else
  set :branch, 'develop'
end
set :vhost, 'dev-create.extension.org'
server 'chowanswamp.vm.extension.org', :app, :web, :db, :primary => true
