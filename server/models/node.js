import mongoose from 'mongoose';

const nodeSchema = new mongoose.Schema({
  key: {
    type: String,
    required: true,
    unique: true,
  },
  name: {
    type: String,
    required: true,
  },
  zone: {
    type: String,
    required: true,
  },
  type: {
    type: String,
    required: true,
    enum: ['probe', 'actuator'],
  },
  status: {
    type: String,
    required: true,
    enum: ['on', 'off'],
    default: 'off',
  },
  history: [{
    timestamp: {
      type: Date,
      required: true,
    },
    action: {
      type: String,
      required: true,
    },
    value: {
      type: String,
      required: true,
    },
  }],
});

const Node = mongoose.model('Node', nodeSchema);

export default Node;
