import { Component, OnInit } from '@angular/core';
import { Router } from '@angular/router';
import { EmployeeFetcherService } from 'src/app/services/employee-fetcher.service';

@Component({
   selector: 'app-login-page',
   templateUrl: './login-page.component.html',
   styleUrls: ['./login-page.component.css']
})
export class LoginPageComponent implements OnInit {
   username: string;
   password: string;

   constructor(private ef: EmployeeFetcherService, private router: Router) { }

   ngOnInit() {
   }

   validateCredentials(): void {
      this.ef.getEmployee(this.username, this.password).subscribe(
         (response) => {
            console.log("Login request delivered.");
            console.log(response);
            if (response) {
               localStorage.setItem("session", JSON.stringify(response));
               if (true === response.isManager)
                  this.router.navigateByUrl('/manager');
               else
                  this.router.navigateByUrl('/employee');
            } else {
               console.log("Invalid login credentials.");
            }
         },

         (response) => {
            console.log("Invalid login credentials.");
         }
      );
   }
}
