import { Injectable } from '@angular/core';
import { Status } from './enums.service';
import { Reimbursement } from '../models/Reimbursement';
import { Observable } from 'rxjs';
import { HttpParams, HttpClient, HttpHeaders } from '@angular/common/http';
import { Employee } from '../models/Employee';
import { imgSrcToBlob } from 'blob-util';

@Injectable({
   providedIn: 'root'
})
export class ReimbursementFetcherService {
   Status = Status;
   constructor(private http: HttpClient) {
   }

   getReimbursements(status: number, username: string): Observable<Reimbursement[]> {
      let e: Employee = JSON.parse(localStorage.getItem("session"));
      const loginUrl = 'http://localhost:8080/Project1/request.do';
      let params = new HttpParams();
      console.log(`s: ${status}, u:  ${e.username}`);
      params = params.set("type", 'get');
      params = params.set("status", status.toString());
      if (!e.isManager)
         params = params.set("username", e.username);
      return this.http.post<Reimbursement[]>(loginUrl, params);
   }

   processReimbursements(status: number, r: Reimbursement): Observable<Reimbursement> {
      let e: Employee = JSON.parse(localStorage.getItem("session"));
      const loginUrl = 'http://localhost:8080/Project1/request.do';
      let params = new HttpParams();
      console.log(`s: ${status}`);
      console.log(r);
      params = params.set("type", "process");
      if (status === 2)
         params = params.set("status", '2');
      else if (status === 3)
         params = params.set("status", "3");
      else {
         console.log("invalid status");
         return null;
      }
      params = params.set("reimbursement", JSON.stringify(r));
      return this.http.post<Reimbursement>(loginUrl, params);
   }

   submitReimbursement(r: Reimbursement): Observable<String> {
      let e: Employee = JSON.parse(localStorage.getItem("session"));
      r.username = e.username;
      const loginUrl = 'http://localhost:8080/Project1/request.do';
      let params = new HttpParams();
      params = params.set("type", "submit");
      return this.http.post<String>(loginUrl, JSON.stringify(r), { 'params': params });
   }
}
