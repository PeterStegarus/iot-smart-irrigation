import jwt from 'jsonwebtoken';
import Node from '../models/node.js';
import dotenv from 'dotenv';
dotenv.config();

const ONE_MINUTE = 60 * 1000;

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

export const postHumidity = async (req, res) => {
  const node = req.user;
  const humidity = req.body.humidity;

  if (!humidity) return res.status(400).json({ message: "Humidity is required" });

  node.history.push({
    timestamp: new Date().toISOString(),
    action: 'humidity',
    value: humidity,
  });
  await node.save();

  return res.status(200).json({ message: "Humidity updated" });
};

export const getHumidity = async (req, res) => {
  const node = req.user;
  const zone = node.zone;
  const currentTime = new Date().getTime();

  try {
    const nodes = await Node.find({ zone: zone.toLowerCase() });
    const recentHumidityData = nodes.flatMap((node) =>
      node.history
        .filter((historyItem) =>
          historyItem.action === 'humidity'
          && currentTime - new Date(historyItem.timestamp).getTime() < ONE_MINUTE
        ).map((historyItem) =>
          historyItem.value
        )
    );

    const averageHumidity = recentHumidityData.reduce((acc, humidity) => acc + parseInt(humidity), 0) / nodes.length;;

    console.log(averageHumidity);

    node.history.push({
      timestamp: new Date().toISOString(),
      action: 'getHumidity',
      value: averageHumidity,
    });

    return res.status(200).json({ humidity: averageHumidity });
  } catch (error) {
    console.log(error);
    return res.status(500).json({ message: "Server error" });
  }
};
