import { blobToBase64String } from 'blob-util';

export class Reimbursement {

   status: number;
   reason: number;
   amount: number;
   userId: number;
   username: string;
   fullName: string;
   timeSubmitted: string;
   timeProcessed: string;
   note: string;
   img: string;

   constructor(status: number, reason: number, amount: number, userId: number,
      username: string, fullName: string, timeSubmitted: string,
      timeProcessed: string, note: string) {

      this.status = status;
      this.reason = reason;
      this.amount = amount;
      this.userId = userId;
      this.username = username;
      this.fullName = fullName;
      this.timeSubmitted = timeSubmitted;
      this.timeProcessed = timeProcessed;
      this.note = note;
      this.img = null;
   }

   addImg(r: Reimbursement, img: File) {
      var reader = new FileReader();
      reader.onloadend = function () {
         let b64 = reader.result as string;
         r.img = b64.substr(b64.indexOf(',') + 1);
      }
      reader.readAsDataURL(img);
   }
   //    let f = new FileReader();
   //    f.onload = this._handleReaderLoaded.bind(this);
   //    f.readAsArrayBuffer(img);
   // }

   // _handleReaderLoaded(event) {
   //    this.img = [];
   //    let ia = new Uint8Array(event.target.result);
   //    let size = ia.length;
   //    ia.forEach((n) => {
   //       this.img.push(n);
   //    })
   //    console.log(this.img);
   // }
}