import { Component, OnInit } from '@angular/core';
import { CommonModule } from '@angular/common';
import { State, Status, Reason } from '../../services/enums.service';
import { Reimbursement } from '../../models/Reimbursement';
import { ReimbursementFetcherService } from 'src/app/services/reimbursement-fetcher.service';
import { Employee } from 'src/app/models/Employee';
import { Router } from '@angular/router';
import { SafeResourceUrl, DomSanitizer } from '@angular/platform-browser';
import { base64StringToBlob } from 'blob-util';

@Component({
   selector: 'app-manager-page',
   templateUrl: './manager-page.component.html',
   styleUrls: ['./manager-page.component.css']
})
export class ManagerPageComponent implements OnInit {
   State = State;
   Status = Status;
   Reason = Reason;
   appHide: boolean = true;
   denHide: boolean = true;
   failHide: boolean = true;
   selectItem: number = 0;
   curState: State;
   sel: Reimbursement;
   sel_pending: boolean;
   reimbursements: Reimbursement[];
   newImg: File = null;
   imgSrc: SafeResourceUrl = null;
   // [
   // new Reimbursement(1, 2, 32.92, 0, 'hjames', 'Henry James', "today", 'N/A', 'new chair'),
   // new Reimbursement(2, 1, 987.23, 18, 'jj', 'jack', 'a week ago', 'yesterday', 'office luncheon')];


   constructor(private rf: ReimbursementFetcherService, private router: Router, private _sanitizer: DomSanitizer) {
      this.curState = this.State.rType;
      this.selectItem = 0;
   }

   ngOnInit() {
   }

   typeReim() {
      this.curState = this.State.rType;
   }

   listReim() {
      this.imgSrc = null;
      let e: Employee = JSON.parse(localStorage.getItem("session"));
      this.rf.getReimbursements(this.selectItem, e.username).subscribe(
         (response) => {
            console.log("Reimbursement request delivered.");
            // console.log(response);
            if (response) {
               this.reimbursements = response;
            } else {
               console.log("Invalid parameters.");
            }
         },

         (response) => {
            console.log("Invalid parameters.");
         }
      );

      this.curState = this.State.rList;
   }

   detailReim(r: Reimbursement) {
      // console.log(r);
      this.sel = r;
      this.sel_pending = this.isPending(this.sel);

      this.appHide = true;
      this.denHide = true;
      this.failHide = true;
      this.curState = this.State.rDetails;
      this.onPrevFileChanged(r);
   }

   processReim(status: number) {
      this.rf.processReimbursements(status, this.sel).subscribe(
         (response) => {
            console.log("process request delivered.");
            // console.log(response);
            if (response) {
               let re: Reimbursement = response;
               if ((status === 3) && (re.status === Status.Approved)) {
                  console.log("Success");

                  this.appHide = false;
                  this.denHide = true;
                  this.failHide = true;
               }
               else if ((status === 2) && (re.status === Status.Denied)) {
                  console.log("Success");

                  this.appHide = true;
                  this.denHide = false;
                  this.failHide = true;
               } else {
                  console.log("Failure");
                  this.appHide = true;
                  this.denHide = true;
                  this.failHide = false;
               }
            } else {
               console.log("Invalid parameters.");
               this.appHide = true;
               this.denHide = true;
               this.failHide = false;
            }
         },

         (response) => {
            console.log("Invalid parameters.");
            this.appHide = true;
            this.denHide = true;
            this.failHide = false;
         }
      );
   }

   approveReim() {
      this.processReim(3);
   }
   denyReim() {
      this.processReim(2);
   }

   isPending(r: Reimbursement): boolean {
      return r.status === 1;
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

