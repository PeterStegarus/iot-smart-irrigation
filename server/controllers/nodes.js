import jwt from 'jsonwebtoken';
import Node from '../models/node.js';
import dotenv from 'dotenv';
dotenv.config();

const createToken = (node) => {
  return jwt.sign({ id: node._id }, process.env.JWT_SECRET, { expiresIn: process.env.JWT_EXPIRATION });
};

export const login = async (req, res) => {
  const node = req.user;

  return res.status(200).json({ token: createToken(node) });
};

export const register = async (req, res) => {
  const body = req.body;
  if (body.name) body.name = body.name.toLowerCase();

  try {
    const node = new Node(req.body);
    await node.save();
    return res.status(201).json({ message: "Node created" });
  } catch (error) {
    if (error.code === 11000) {
      return res.status(409).json({ message: "Node already exists" });
    }

    return res.status(500).json({ message: "Server error" });
  }
};
