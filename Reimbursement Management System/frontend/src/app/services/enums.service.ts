import { Injectable } from '@angular/core';

// @Injectable({
//   providedIn: 'root'
// })
// export class EnumsService {
//    State = State;
//    Reason = Reason;
//    Status = Status;
//   constructor() { }

//    getState(index:number) {
//       return this.State[index];
//    }
//    getReason(index:number) {
//       return this.Reason[index];
//    }
//    getStatus(index:number) {
//       return this.Status[index];
//    }
// }

export enum Status { Pending = 1, Denied, Approved };
export enum Reason { Food = 1, Supplies, Travel, Shipping, Other };
export enum State { rType = 1, rList, rDetails, rChoice, rSubmit };