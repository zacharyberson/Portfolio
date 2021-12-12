import { Component, OnInit } from '@angular/core';
import { State, Status, Reason } from 'src/app/services/enums.service';
import { Reimbursement } from 'src/app/models/Reimbursement';
import { ReimbursementFetcherService } from 'src/app/services/reimbursement-fetcher.service';
import { Router } from '@angular/router';
import { Employee } from 'src/app/models/Employee';
import { TagContentType } from '@angular/compiler';
import { DomSanitizer, SafeStyle, SafeResourceUrl } from '@angular/platform-browser';
import { base64StringToBlob, blobToBase64String } from 'blob-util';

@Component({
   selector: 'app-employee-page',
   templateUrl: './employee-page.component.html',
   styleUrls: ['./employee-page.component.css']
})
export class EmployeePageComponent implements OnInit {
   State = State;
   Status = Status;
   Reason = Reason;
   goodHide: boolean = true;
   failHide: boolean = true;
   fullName: string;
   e: Employee;
   selectItem: number = 0;
   curState: State;
   sel: Reimbursement;
   reimbursements: Reimbursement[];
   newReim: Reimbursement;
   newAmount: string;
   newImg: File = null;
   imgSrc: SafeResourceUrl = null;

   constructor(private rf: ReimbursementFetcherService, private router: Router, private _sanitizer: DomSanitizer) {
      this.curState = this.State.rChoice;
      this.selectItem = 0;
      this.e = JSON.parse(localStorage.getItem("session"));
      this.newImg = null;
      this.imgSrc = null;
   }

   ngOnInit() {
      this.newReim = new Reimbursement(1, 1, 0, 0, "", "", "", "", "");
   }

   choices() {
      this.curState = State.rChoice;
   }

   viewHistory() {
      this.goodHide = true;
      this.failHide = true;
      this.curState = State.rType;
   }

   submitForm() {
      this.goodHide = true;
      this.failHide = true;
      this.curState = State.rSubmit;
      this.newImg = null;
   }

   listReim() {
      this.imgSrc = null;
      let e: Employee = JSON.parse(localStorage.getItem("session"));
      // console.log(this.selectItem);
      // console.log(e.username);
      this.rf.getReimbursements(this.selectItem, e.username).subscribe(
         (response) => {
            console.log("Reimbursement request delivered.");
            // console.log(response);
            if (response) {
               this.reimbursements = response;
            } else {
               console.log("Request Failed.");
            }
         },

         (response) => {
            console.log("Invalid parameters.");
         }
      );
      // if (null !== this.imgSrc)
      //    window.URL.revokeObjectURL(this.imgSrc);
      this.imgSrc = null;
      this.curState = this.State.rList;
   }

   detailReim(r: Reimbursement) {
      // console.log(r);
      this.sel = r;

      this.curState = this.State.rDetails;
      this.onPrevFileChanged(r);
      // this.imgDisplay(r);
   }

   submitReimbursement() {
      this.newReim.amount = parseFloat(this.newAmount);
      // console.log("sending this: ");
      // console.log(this.newReim);
      this.rf.submitReimbursement(this.newReim).subscribe(
         (response) => {
            console.log("submit request delivered.");
            // console.log(response);
            if (response) {
               if ("true" === response.toString()) {
                  console.log("Success");
                  this.goodHide = false;
                  this.failHide = true;
                  this.curState = State.rChoice;
               } else {
                  console.log("Failure");
                  this.goodHide = true;
                  this.failHide = false;
               }
            } else {
               console.log("Invalid parameters.");
               this.goodHide = true;
               this.failHide = false;
            }
         },

         (response) => {
            console.log("Invalid parameters.");
            this.goodHide = true;
            this.failHide = false;
         }
      );
   }

   onFileChanged(event) {
      this.newImg = event.target.files[0];
      if (null !== this.newImg)
         this.newReim.addImg(this.newReim, this.newImg);



   }

   onPrevFileChanged(r: Reimbursement) {
      // this.imgSrc = event.target.files[0];
      // let reader = new FileReader();

      // reader.onload = (e: any) => {
      //    this.imgSrc = e.target.result;
      // }

      // reader.readAsDataURL(event.target.files[0]);
      // this.imgSrc = this._sanitizer.bypassSecurityTrustStyle('linear-gradient(rgba(29, 29, 29, 0), rgba(16, 16, 23, 0.5)), url(' + URL.createObjectURL(new Blob([(new Int8Array(r.img))])));
      // let nfnf = new Uint8Array(r.img.length);
      // nfnf.set(r.img);
      // console.log(r.img);
      // console.log(r.img)
      if (null !== r.img && undefined !== r.img)
         this.imgSrc = URL.createObjectURL(base64StringToBlob(r.img, "image/jpg"));
      else
         this.imgSrc = null;
   }



   // imgDisplay(r: Reimbursement) {
   //    // // let urlCreator = window.URL || window.webkitURL;
   //    // let newUrl = window.URL;
   //    // let imageUrl = newUrl.createObjectURL(r.img);
   //    // this.imgSrc = imageUrl;
   // }
   cleanURL(oldURL: string): SafeResourceUrl {
      if (null === oldURL)
         return null;
      else
         return this._sanitizer.bypassSecurityTrustResourceUrl(oldURL);
   }
   logout() {
      localStorage.setItem("session", null);
      this.newImg = null;
      this.imgSrc = null;
      this.router.navigateByUrl("/login");
   }

   notNull() {
      return null !== this.imgSrc;
   }
}
