
// 

const noteNames = ["note_c", "note_cs", "note_d", "note_ds", "note_e", "note_f", "note_fs", "note_g", "note_gs", "note_a", "note_as", "note_b"];
let noteActive = [false, false, false, false, false, false, false, false, false, false, false, false]

let scaleList = [];

function sectionClicked(section) {

   if (section < 0)
      scaleList = [];
   else
      scaleList[0] = section;

   canvas.update();
}

function noteClicked(note) {

   let index = noteNames.indexOf(note);
   noteActive[index] = noteActive[index] ? false : true;

   canvas.update();
}

//

class ClickBox {

   constructor(x, y, width, height, layer) {

      this.x = x;
      this.y = y;
      this.width = width;
      this.height = height;

      this.layer = layer;
   }

   contains(x, y) {

      if (x < this.x)
         return false;
      if (x > this.x + this.width)
         return false;

      if (y < this.y)
         return false;
      if (y > this.y + this.height)
         return false;

      return true;
   }
}

class CircleOfFiths extends Canvas {

   constructor(parent) {

      super(parent, 250, 400);

      this.img = new Image();
      this.img.src = "./tuning/CircleOfFiths.svg";

      this.ctx.font = "bold 16px Arial";
      this.ctx.textAlign = "left";

      this.element.addEventListener("pointerdown", (clickEvent) => {
         this.#clicked(Math.round(clickEvent.offsetX), Math.round(clickEvent.offsetY));
      });

      this.pieSize = (360 / 12);
      this.pieOffset = 8.5 * this.pieSize;

      this.cx = 125;
      this.cy = 242;
      this.clickBoxes = {};
      this.clickBoxes["scale"] = new ClickBox(0, this.cy - 122, 250, 250, 0);

      // white keys
      const kx = 20;
      const ky = 30;
      const keyWidth = 30;
      const keyHeight = 80;

      this.clickBoxes["note_c"] = new ClickBox(kx + 0 * keyWidth, ky, keyWidth, keyHeight, 0);
      this.clickBoxes["note_d"] = new ClickBox(kx + 1 * keyWidth, ky, keyWidth, keyHeight, 0);
      this.clickBoxes["note_e"] = new ClickBox(kx + 2 * keyWidth, ky, keyWidth, keyHeight, 0);

      this.clickBoxes["note_f"] = new ClickBox(kx + 3 * keyWidth, ky, keyWidth, keyHeight, 0);
      this.clickBoxes["note_g"] = new ClickBox(kx + 4 * keyWidth, ky, keyWidth, keyHeight, 0);
      this.clickBoxes["note_a"] = new ClickBox(kx + 5 * keyWidth, ky, keyWidth, keyHeight, 0);
      this.clickBoxes["note_b"] = new ClickBox(kx + 6 * keyWidth, ky, keyWidth, keyHeight, 0);

      // black keys
      this.clickBoxes["note_cs"] = new ClickBox(kx + 2 + 0.5 * keyWidth, ky, keyWidth - 4, 0.7 * keyHeight, 1);
      this.clickBoxes["note_ds"] = new ClickBox(kx + 2 + 1.5 * keyWidth, ky, keyWidth - 4, 0.7 * keyHeight, 1);

      this.clickBoxes["note_fs"] = new ClickBox(kx + 2 + 3.5 * keyWidth, ky, keyWidth - 4, 0.7 * keyHeight, 1);
      this.clickBoxes["note_gs"] = new ClickBox(kx + 2 + 4.5 * keyWidth, ky, keyWidth - 4, 0.7 * keyHeight, 1);
      this.clickBoxes["note_as"] = new ClickBox(kx + 2 + 5.5 * keyWidth, ky, keyWidth - 4, 0.7 * keyHeight, 1);

      // finalise
      this.img.onload = () => {
         this.update();
      }
   }

   update() {

      this.clear();

      this.ctx.fillStyle = "#444444";
      this.ctx.fillText("Name", 5, 20);

      this.#drawKeys();

      if (0 == scaleList.length)
         this.circle(125, this.cy, 25, "#cccccc");
      else {
         for (let index in scaleList)
            this.#drawMarker(scaleList[index]);

      }

      this.ctx.drawImage(this.img, 0, this.cy - 122, 250, 250);

      this.ctx.fillStyle = "#444444";
      this.ctx.fillText("Visu", 10, this.cy + 150);
   }


   #clicked(x, y) {

      let hitbox = undefined;
      let hitkey = undefined;

      for (let key in this.clickBoxes) {
         let cbox = this.clickBoxes[key];
         if (!cbox.contains(x, y))
            continue;

         if (!hitbox || hitbox.layer < cbox.layer) {
            hitkey = key;
            hitbox = cbox;
         }
      }

      if (!hitkey)
         return;

      if (hitkey.startsWith("note_"))
         noteClicked(hitkey);
      else if ("scale" == hitkey) {
         let cx = x - hitbox.x - 125;
         let cy = y - hitbox.y - 125;
         let radius = Math.sqrt(cx * cx + cy * cy);
         if (radius < 25) {
            sectionClicked(-1);
         }
         else {
            let angle = Math.atan2(cy, cx) * (180 / Math.PI);
            let sector = Math.round(angle / this.pieSize) + 3;
            if (sector < 0)
               sector += 12;
            sectionClicked(sector);
         }
      }
   }

   #polar(radius, angle) {

      let rad = angle * (Math.PI / 180);
      let x = radius * Math.cos(rad);
      let y = radius * Math.sin(rad);

      return [x, y];
   }

   #drawMarker(sector) {

      let startAngle = this.pieOffset + (sector + 0) * this.pieSize;
      let endAngle = this.pieOffset + (sector + 1) * this.pieSize;

      this.ctx.fillStyle = "#cccccc";

      this.ctx.beginPath();

      let [x0, y0] = this.#polar(30, startAngle);
      this.ctx.moveTo(this.cx + x0, this.cy + y0);

      let diffAngle = endAngle - startAngle;
      for (let fraction = 0.0; fraction < 1.0; fraction += 0.1) {
         let [x1, y1] = this.#polar(125, startAngle + fraction * diffAngle);
         this.ctx.lineTo(this.cx + x1, this.cy + y1);
      }

      let [x2, y2] = this.#polar(125, endAngle);
      this.ctx.lineTo(this.cx + x2, this.cy + y2);

      let [x3, y3] = this.#polar(30, endAngle);
      this.ctx.lineTo(this.cx + x3, this.cy + y3);

      this.ctx.lineTo(this.cx + x0, this.cy + y0);
      this.ctx.fill();

      this.ctx.closePath();
   }

   #drawKeys() {

      for (let key in this.clickBoxes) {
         if (!key.startsWith("note_"))
            continue;

         let index = noteNames.indexOf(key);
         let active = noteActive[index];

         let cbox = this.clickBoxes[key];

         let color = undefined;
         if (1 == cbox.layer)
            color = active ? "#444444" : "#888888";
         else
            color = active ? "#eeeeee" : "#bbbbbb";

         this.box(cbox.x, cbox.y, cbox.width, cbox.height, color, "#444444");
      }
   }
};

//
setupDocument(264, 1, 1);
let title = new Title("Qunatizer");

let main = new Div();
main.setStyle("background", "#444444");
main.setStyle("padding", "5px");

let canvas = new CircleOfFiths(main);
canvas.update();

let nameEdit = new TextEdit(main);
nameEdit.onChange(nameChanged);
nameEdit.move(60, 28);

let clearButton = new Button(main, "clear");
clearButton.onClicked(clearVisu);
clearButton.move(60, 400);

let resendButton = new Button(main, "resend");
resendButton.onClicked(resendVisu);
resendButton.move(110, 400);

// name

max.bindInlet('setName', setName);
function setName(name) {

   nameEdit.setText(name);
}

function nameChanged(name) {

   max.outlet("updateName", name);
}

// visu

function clearVisu() {

   max.outlet("clear");
}

function resendVisu() {

   max.outlet("visu");
}

