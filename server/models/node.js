import mongoose from 'mongoose';

const nodeSchema = new mongoose.Schema({
  name: {
    type: String,
    required: true,
    index: true,
  },
  zone: {
    type: String,
    required: true,
    index: true,
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

nodeSchema.index({ 'name': 1, 'zone': 1 }, { unique: true });

const Node = mongoose.model('Node', nodeSchema);

export default Node;
