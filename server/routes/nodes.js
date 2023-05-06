import express from 'express';
import passport from 'passport';
import { login, register, postHumidity, getHumidity } from '../controllers/nodes.js';

const router = express.Router();

router.post('/login', passport.authenticate('local', { session: false }), login);
router.post('/register', register);
router.post('/humidity', passport.authenticate('jwt', { session: false }), postHumidity);
router.get('/humidity', passport.authenticate('jwt', { session: false }), getHumidity);

export default router;
