import express from 'express';
import cors from 'cors';
import mongoose from 'mongoose';
import morgan from 'morgan';
import dotenv from 'dotenv';
import _ from 'lodash';

// Import authentication middleware
import passport from 'passport';
import LocalStrategy from 'passport-local';
import passportJWT from 'passport-jwt';
const JwtStrategy = passportJWT.Strategy;
const ExtractJwt = passportJWT.ExtractJwt;

// Import models
import Node from './models/node.js';
import Zone from './models/zone.js';

// Import routes
import indexRoutes from './routes/index.js';
import nodesRoutes from './routes/nodes.js';

// Load environment variables from .env file
dotenv.config();

const app = express();
app.use(cors());          // Enable CORS middleware
app.use(morgan('dev'));   // Enable logging middleware
app.use(express.json());  // Parse incoming JSON data

passport.use(
  new LocalStrategy({ usernameField: 'name', passwordField: 'zone' }, async (name, zone, done) => {
    try {
      const node = await Node.findOne({ name: name.toLowerCase(), zone: zone.toLowerCase() });
      if (!node) {
        return done(null, false, { message: 'Node not found' });
      }

      return done(null, node);
    } catch (error) {
      return done(error);
    }
  })
);

passport.use(
  new JwtStrategy(
    {
      jwtFromRequest: ExtractJwt.fromAuthHeaderAsBearerToken(),
      secretOrKey: process.env.JWT_SECRET,
    },
    async (payload, done) => {
      try {
        const node = await Node.findById(payload.id);

        return node
          ? done(null, node)
          : done(null, false, { message: 'Node not found' });
      } catch (error) {
        return done(error);
      }
    }
  )
);

// Connect to the MongoDB database
mongoose.connect(process.env.MONGODB_URI, {
  useNewUrlParser: true,
  useUnifiedTopology: true,
}).then(() => {
  console.log('Connected to MongoDB');
}).catch((err) => {
  console.error(err);
});

// Set up routes
app.use('/api/media', express.static('media'));
app.use('/api', indexRoutes);
app.use('/api/nodes', nodesRoutes);

// Start the server
const PORT = process.env.PORT;
app.listen(PORT, () => {
  console.log(`Server listening on port ${PORT}`);
});
