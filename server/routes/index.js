import express from 'express';
import passport from 'passport';
const router = express.Router();

router.get('/hello', (req, res) => {
  res.json({ message: 'Hello, world!' });
});

router.get('/protected-hello', passport.authenticate('jwt', { session: false }), (req, res) => {
  res.json({ message: 'Hello, world!' });
});

export default router;