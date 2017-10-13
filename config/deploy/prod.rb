set :deploy_to, "/services/create/"
set :branch, 'master'
set :vhost, 'create.extension.org'
set :deploy_server, 'create.awsi.extension.org'
server deploy_server, :app, :web, :db, :primary => true
