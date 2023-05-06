sleep 3
echo "Initiating mongodb replica set ( ~ 1 minute )"
mongosh mongodb://admin:admin@127.0.0.1:27018 --eval 'rs.initiate({_id: "rs0", members: [{ _id: 0, host: "mongodb-irrigation-system:27017" }]})'
