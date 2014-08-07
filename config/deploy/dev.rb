set :deploy_to, '/services/apache/vhosts/dev.create.extension.org/docroot/'
server 'dev.create.extension.org', :app, :web, :db, :primary => true
if(branch = ENV['BRANCH'])
  set :branch, branch
else
  set :branch, 'develop'
end
