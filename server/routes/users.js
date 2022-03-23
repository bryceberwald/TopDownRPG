var express = require('express');
var router = express.Router();

/* GET users listing. */
router.get('/', function(req, res, next) {
  res.send('this fucking works');
});

router.post('/', function(req, res, next) {
  res.send("Hello from server!!!");
  console.log(req.body); // Log JSON data sent from c++ client
});

module.exports = router;
