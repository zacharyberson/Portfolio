import { TestBed } from '@angular/core/testing';

import { ReimbursementFetcherService } from './reimbursement-fetcher.service';

describe('ReimbursementFetcherService', () => {
  beforeEach(() => TestBed.configureTestingModule({}));

  it('should be created', () => {
    const service: ReimbursementFetcherService = TestBed.get(ReimbursementFetcherService);
    expect(service).toBeTruthy();
  });
});
