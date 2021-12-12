import { TestBed } from '@angular/core/testing';

import { EnumsService } from './enums.service';

describe('EnumsService', () => {
  beforeEach(() => TestBed.configureTestingModule({}));

  it('should be created', () => {
    const service: EnumsService = TestBed.get(EnumsService);
    expect(service).toBeTruthy();
  });
});
