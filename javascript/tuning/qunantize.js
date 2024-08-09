autowatch = 1;

// inlets and outlets
inlets = 1;
setinletassist(0, "midinote, message");

outlets = 3;
setoutletassist(0, "midinote");
setoutletassist(1, "visu");
setoutletassist(2, "ui message");

//////////////////////////////////////////

var settings = {};

function getvalueof() {
   var text = JSON.stringify(settings);
   return text;
}

function setvalueof(value) {
   settings = JSON.parse(value);

   scaleName = settings["name"];
   if (!scaleName)
      scaleName = "main";


}

//////////////////////////////////////////

include("_scale.js");

var scales = new Global("Scales");
var scaleName = "main";

function updateName(text) {

   scaleName = text;
   settings["name"] = text;
}

function bang() {

   outlet(2, ["setName", scaleName]);
}

function name(text) {

   scaleName = text;
}

function msg_int(midiNote) {

   if (scales.target === undefined) {
      post("no target", "\n");
      return out(midiNote);
   }

   if (scales.target[scaleName] === undefined) {
      post("no scale", scaleName, "\n");
      return out(midiNote);
   }


   midiNote = scales.target[scaleName].closestMatch(midiNote);
   out(midiNote);
}

out.local = 1;
function out(midiNote) {

   outlet(0, midiNote);
}

