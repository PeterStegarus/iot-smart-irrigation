import express from 'express';
import passport from 'passport';
import { login, register } from '../controllers/nodes.js';

const router = express.Router();

router.post('/login', passport.authenticate('local', { session: false }), login);
router.post('/register', register);

export default router;
