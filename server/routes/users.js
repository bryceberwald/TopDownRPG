var express = require('express');
var router = express.Router();

const mongoose = require('mongoose');
const User = require('../models/User');



/* GET users listing. */
router.get('/', async function(req, res, next) {
  //res.send('this fucking works');
  
  var currentUser = {
    username: "",
    password: "",
  }

  currentUser.username = req.body.Key1;
  currentUser.password = req.body.Key2;

  await User.find({ username: currentUser.username, password: currentUser.password}, function (err, docs) {
    if (err){
        console.log(err);
    }
    else{
        console.log("GET REQUEST - Found User.", docs);
    }
  });

});

/* POST users listing. */
router.post('/', async function(req, res, next) {

  var newUser = {
    username: "",
    password: "",
  }

  newUser.username = req.body.Key1;
  newUser.password = req.body.Key2;

  if(newUser.username.length > 0 && newUser.password.length > 0){
    await new User(newUser).save();
    res.send("Account Created!");
  } else {
    res.send("Must have a username AND password...");
  }

});

module.exports = router;
