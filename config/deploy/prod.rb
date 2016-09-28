set :deploy_to, "/services/create/"
set :branch, 'master'
set :vhost, 'create.extension.org'
server vhost, :app, :web, :db, :primary => true
set :port, 24
