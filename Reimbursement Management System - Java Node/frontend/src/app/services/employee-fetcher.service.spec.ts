import { TestBed } from '@angular/core/testing';

import { EmployeeFetcherService } from './employee-fetcher.service';

describe('EmployeeFetcherService', () => {
  beforeEach(() => TestBed.configureTestingModule({}));

  it('should be created', () => {
    const service: EmployeeFetcherService = TestBed.get(EmployeeFetcherService);
    expect(service).toBeTruthy();
  });
});
