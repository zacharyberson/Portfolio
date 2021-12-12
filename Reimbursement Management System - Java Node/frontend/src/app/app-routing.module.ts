import { NgModule } from '@angular/core';
import { Routes, RouterModule } from '@angular/router';
import { EmployeePageComponent } from './components/employee-page/employee-page.component';
import { ManagerPageComponent } from './components/manager-page/manager-page.component';
import { LoginPageComponent } from './components/login-page/login-page.component';

const routes: Routes = [
   { path: '', redirectTo: '/login', pathMatch: 'full' },
   { path: 'employee', component: EmployeePageComponent },
   { path: 'manager', component: ManagerPageComponent },
   { path: 'login', component: LoginPageComponent }
];

@NgModule({
   imports: [RouterModule.forRoot(routes)],
   exports: [RouterModule]
})
export class AppRoutingModule { }
