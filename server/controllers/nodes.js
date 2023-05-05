import jwt from 'jsonwebtoken';
import Node from '../models/node.js';
import dotenv from 'dotenv';
dotenv.config();

const createToken = (node) => {
  return jwt.sign({ id: node._id }, process.env.JWT_SECRET, { expiresIn: process.env.JWT_EXPIRATION });
};

export const login = async (req, res) => {
  const node = req.user;

  node.status = 'on';
  node.history.push({
    timestamp: new Date().toISOString(),
    action: 'login',
    value: node.key,
  });
  await node.save();

  return res.status(200).json({ token: createToken(node) });
};

export const register = async (req, res) => {
  const body = req.body;
  if (body.name) body.name = body.name.toLowerCase();

  const key = `<${body.name}>_<${body.zone}>`;

  try {
    await new Node({
      key,
      ...body,
      history: [{
        timestamp: new Date().toISOString(),
        action: 'register',
        value: key,
      }]
    }).save();

    return res.status(201).json({ message: "Node created" });
  } catch (error) {
    return error.code === 11000
      ? res.status(409).json({ message: "Node already exists" })
      : res.status(500).json({ message: "Server error" });
  }
};
