import mongoose from 'mongoose';

const zoneSchema = new mongoose.Schema({
  name: {
    type: String,
    required: true,
  },
  zone: {
    type: String,
    required: true,
  }
});

const Zone = mongoose.model('Zone', zoneSchema);

export default Zone;
