import { Injectable } from '@angular/core';
import { Employee } from '../models/Employee';
import { Observable } from 'rxjs';
import { HttpParams, HttpClient } from '@angular/common/http';

@Injectable({
   providedIn: 'root'
})
export class EmployeeFetcherService {


   constructor(private http: HttpClient) { }

   getEmployee(username: string, password: string): Observable<Employee> {
      const loginUrl = 'http://localhost:8080/Project1/login.do';
      let params = new HttpParams();
      console.log(`u: ${username}, p:  ${password}`);
      params = params.set("username", username);
      params = params.set("password", password);
      return this.http.post<Employee>(loginUrl, params);
   }
}
