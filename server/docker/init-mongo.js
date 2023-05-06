db.getSiblingDB('admin').auth(
  process.env.MONGO_INITDB_ROOT_USERNAME,
  process.env.MONGO_INITDB_ROOT_PASSWORD
);

db = db.getSiblingDB(process.env.MONGO_USER_DATABASE);
db.createUser({
  user: process.env.MONGO_USER_USERNAME,
  pwd: process.env.MONGO_USER_PASSWORD,
  roles: [{
    role: "readWrite",
    db: process.env.MONGO_USER_DATABASE
  }]
});

db.status.insertOne({ health: "OK" });
