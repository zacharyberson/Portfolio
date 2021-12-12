package dev.zberson.eval;



import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertTrue;

import org.junit.Rule;
import org.junit.Test;
import org.junit.rules.ExpectedException;


import dev.zberson.service.EmployeeService;
import dev.zberson.service.EmployeeServiceImpl;
import dev.zberson.service.RequestService;
import dev.zberson.service.RequestServiceImpl;

public class TestService {
    @Rule
    public ExpectedException expectedException = ExpectedException.none();
    
    /*******************************************
     * Service Tests
     *******************************************/
    
    EmployeeService es = new EmployeeServiceImpl();
    RequestService rs = new RequestServiceImpl();
//
//    @Test
//    public void testServiceCheckCredentialsValidUser() {
//        assertTrue(es.checkCredentials("Curtis12", "password", false));
//    }
//    
//    @Test
//    public void testServiceCheckCredentialsValidManager() {
//        assertTrue(es.checkCredentials("mrbossman", "supersecret", true));
//    }
//    
//    @Test
//    public void testServiceCheckCredentialsInvalidUser() {
//        assertFalse(es.checkCredentials("Curtis12", "supersecret", false));
//    }
//    
//    @Test
//    public void testServiceCheckCredentialsInvalidManager() {
//        assertFalse(es.checkCredentials("mrbossman", "password", true));
//    }
//
//    @Test
//    public void testServiceCheckCredentialsInvalidUsername() {
//        assertFalse(es.checkCredentials("nobody", "password", false));
//    }
//
//    @Test
//    public void testServiceGetRequestsPendingUserFullList() {
//        assertTrue(0 < rs.getRequests(1, "James").size());
//    }
//    
//    @Test
//    public void testServiceGetRequestsPendingUserEmptyList() {
//        assertTrue(0 == rs.getRequests(1, "mrbossman").size());
//    }
//    
//    @Test
//    public void testServiceGetRequestsPendingWrongUserEmptyList() {
//        assertTrue(0 == rs.getRequests(1, "mrbossman2").size());
//    }
//    
//    @Test
//    public void testServiceGetRequestsApprovedUserFullList() {
//        assertTrue(0 < rs.getRequests(3, "Ben123").size());
//    }
//    
//    @Test
//    public void testServiceGetRequestsApprovedUserEmptyList() {
//        assertEquals(0, rs.getRequests(3, "conway321").size());
//    }
//    
//    @Test
//    public void testServiceGetRequestsApprovedWrongUserEmptyList() {
//        assertTrue(0 == rs.getRequests(3, "mrbossman2").size());
//    }
//
//    @Test
//    public void testServiceGetRequestsDeniedUserFullList() {
//        assertTrue(0 < rs.getRequests(2, "mrbossman").size());
//    }
//    
//    @Test
//    public void testServiceGetRequestsDeniedUserEmptyList() {
//        assertEquals(0, rs.getRequests(2, "Ben123").size());
//    }
//    
//    @Test
//    public void testServiceGetRequestsDeniedWrongUserEmptyList() {
//        assertTrue(0 == rs.getRequests(2, "mrbossman2").size());
//    }
//    
//    @Test
//    public void testServiceGetRequestsPendingAllFullList() {
//        assertTrue(0 < rs.getRequests(1, null).size());
//    }
//    
//    @Test
//    public void testServiceGetRequestsApprovedAllFullList() {
//        assertTrue(0 < rs.getRequests(3, null).size());
//    }
//    
//    @Test
//    public void testServiceGetRequestsDeniedAllFullList() {
//        assertTrue(0 < rs.getRequests(2, null).size());
//    }
//    
//    @Test
//    public void testServiceSubmitRequest() {
//        assertTrue(rs.submitRequest(1, 23.43, "Curtis12", null));
//    }
//    
//    @Test
//    public void testServiceApproveRequestValid() {
//        assertTrue(rs.approveRequest("James", "08-22-2019 23:11:15"));
//    }
//    
//    @Test
//    public void testServiceApproveRequestInvalid() {
//        assertFalse(rs.approveRequest("James", "02-19-2015 11:11:11"));
//    }
//    
//    @Test
//    public void testServiceDenyRequestValid() {
//        assertTrue(rs.approveRequest("Curtis12", "02-03-2019 02:13:02"));
//    }
//    
//    @Test
//    public void testServiceDenyRequestInvalid() {
//        assertFalse(rs.approveRequest("Curtis12", "01-01-1990 01:01:01"));
//    }
}
