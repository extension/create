set :deploy_to, "/services/create/"
if(branch = ENV['BRANCH'])
  set :branch, branch
else
  set :branch, 'master'
end
set :vhost, 'dev-create.extension.org'
set :deploy_server, 'dev-create.aws.extension.org'
server deploy_server, :app, :web, :db, :primary => true
set :port, 22
