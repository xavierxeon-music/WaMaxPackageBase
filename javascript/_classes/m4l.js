// max for live helpoer

if (typeof this.M4L === "undefined") {

   this.M4L = class {

      static apiPrint(api) {

         if (api == undefined) {
            print("UNDEFINED");
            return;
         }

         for (var key in api) {
            // if ("info" == key)
            //    continue;
            var value = api[key];
            print("KEY", key,
               "T", typeof value,
               "V", value);
         }
      }

      static dictParse(api, key) {

         var target = {};

         var channelDict = JSON.parse(api.get(key)[0]);
         var channelList = channelDict[key];

         var channelCount = api.getcount(key);
         for (var channelCounter = 0; channelCounter < channelCount; channelCounter++) {
            var channelObject = channelList[channelCounter];
            var name = channelObject["display_name"];
            var id = channelObject["identifier"];
            target[name] = id;
         }

         return target;
      }
   }
}