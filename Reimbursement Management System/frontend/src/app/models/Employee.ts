export class Employee {

    fullName:string;
    username:string;
    password:string;
    isManager:boolean;

    constructor(fullName:string, username:string,
        password:string, isManager:boolean) {
        this.fullName = fullName;
        this.username = username;
        this.password = password;
        this.isManager = isManager;
    }
}