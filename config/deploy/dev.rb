set :deploy_to, "/services/create/"
if(branch = ENV['BRANCH'])
  set :branch, branch
else
  set :branch, 'master'
end
set :vhost, 'dev-create.extension.org'
server vhost, :app, :web, :db, :primary => true
