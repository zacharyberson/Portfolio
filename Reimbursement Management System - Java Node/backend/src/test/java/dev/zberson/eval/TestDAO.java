package dev.zberson.eval;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertTrue;
import static org.junit.Assert.assertNull;
import static org.junit.Assert.assertNotNull;

import java.sql.SQLException;
import java.sql.SQLIntegrityConstraintViolationException;
import java.sql.SQLTimeoutException;
import java.util.List;

import org.junit.Rule;
import org.junit.Test;
import org.junit.rules.ExpectedException;


import dev.zberson.model.Request;
import dev.zberson.model.Employee;
import dev.zberson.dao.EmployeeDAO;
import dev.zberson.dao.EmployeeDAOImpl;
import dev.zberson.dao.RequestDAO;
import dev.zberson.dao.RequestDAOImpl;
import dev.zberson.exception.CustomException.RequestAlreadyProcessedException;

public class TestDAO {
    
    @Rule
    public ExpectedException expectedException = ExpectedException.none();
    
    /*******************************************
     * DAO Tests
     *******************************************/
    RequestDAO rDao = new RequestDAOImpl();
    EmployeeDAO eDao = new EmployeeDAOImpl();

    @Test
    public void testGetRequestsPendingUserFullList() {
        assertTrue(0 < rDao.getRequests(new Request(1, "conway321")).size());
    }
    
    @Test
    public void testGetRequestsPendingUserEmptyList() {
        assertTrue(0 == rDao.getRequests(new Request(1, "mrbossman")).size());
    }
    
    @Test
    public void testGetRequestsPendingWrongUserEmptyList() {
        assertTrue(0 == rDao.getRequests(new Request(1, "mrbossman2")).size());
    }
    
    @Test
    public void testGetRequestsApprovedUserFullList() {
        assertTrue(0 < rDao.getRequests(new Request(3, "Ben123")).size());
    }
    
    @Test
    public void testGetRequestsApprovedUserEmptyList() {
        assertEquals(0, rDao.getRequests(new Request(3, "conway321")).size());
    }
    
    @Test
    public void testGetRequestsApprovedWrongUserEmptyList() {
        assertTrue(0 == rDao.getRequests(new Request(3, "mrbossman2")).size());
    }

    @Test
    public void testGetRequestsDeniedUserFullList() {
        assertTrue(0 < rDao.getRequests(new Request(2, "mrbossman")).size());
    }
    
    @Test
    public void testGetRequestsDeniedUserEmptyList() {
        assertEquals(0, rDao.getRequests(new Request(2, "Ben123")).size());
    }
    
    @Test
    public void testGetRequestsDeniedWrongUserEmptyList() {
        assertTrue(0 == rDao.getRequests(new Request(2, "mrbossman2")).size());
    }
    
    @Test
    public void testGetRequestsPendingAllFullList() {
        assertTrue(0 < rDao.getRequests(new Request(1, null)).size());
    }
    
    @Test
    public void testGetRequestsApprovedAllFullList() {
        assertTrue(0 < rDao.getRequests(new Request(3, null)).size());
    }
    
    @Test
    public void testGetRequestsDeniedAllFullList() {
        assertTrue(0 < rDao.getRequests(new Request(2, null)).size());
    }
    
    @Test
    public void testSubmitRequest() throws SQLIntegrityConstraintViolationException, SQLTimeoutException, SQLException {
        rDao.submitRequest(new Request(1, 2, 24.40, "conway321"));
    }
    
    @Test
    public void testSubmitRequestNote() throws SQLIntegrityConstraintViolationException, SQLTimeoutException, SQLException {
        rDao.submitRequest(new Request(1, 5, 377.90, "conway321", "note"));
    }
    
    @Test
    public void testProcessRequestApprove() throws SQLIntegrityConstraintViolationException, SQLTimeoutException, SQLException, RequestAlreadyProcessedException {
        rDao.processRequest(new Request(3, "Curtis12", "02-03-2019 02:13:02"));
        List<Request> lr = rDao.getRequests(new Request(3, "Curtis12"));
        assertEquals(3, lr.get(0).getStatus());
    }
    
    @Test
    public void testProcessRequestDeny() throws SQLIntegrityConstraintViolationException, SQLTimeoutException, SQLException, RequestAlreadyProcessedException {
        rDao.processRequest(new Request(2, "James", "08-22-2019 23:11:15"));
        assertEquals(2, rDao.getRequests(new Request(2, "Curtis12")).get(0).getStatus());
    }
    
    @Test
    public void testProcessRequestAlreadyProcessed() throws SQLIntegrityConstraintViolationException, SQLTimeoutException, SQLException, RequestAlreadyProcessedException {
        expectedException.expect(RequestAlreadyProcessedException.class);
        rDao.processRequest(new Request(2, "Curtis12", "02-03-2019 02:13:02"));
    }
    
    @Test
    public void testCheckCredentialsValidEmployee() throws SQLTimeoutException, SQLException {
        Employee e = eDao.checkCredentials(new Employee(null, "James", "password", false));
        assertNotNull(e);
        assertEquals("James", e.getUsername());
    }
    
    @Test
    public void testCheckCredentialsInvalidEmployee() throws SQLTimeoutException, SQLException {
        Employee e = eDao.checkCredentials(new Employee(null, "James", "wrong", false));
        assertNull(e);
    }
    
    @Test
    public void testCheckCredentialsValidManager() throws SQLTimeoutException, SQLException {
        Employee e = eDao.checkCredentials(new Employee(null, "mrbossman", "supersecret", true));
        assertNotNull(e);
        assertEquals("mrbossman", e.getUsername());
    }
    
    @Test
    public void testCheckCredentialsInvalidManager() throws SQLTimeoutException, SQLException {
        Employee e = eDao.checkCredentials(new Employee(null, "mrbossman", "password", true));
        assertNull(e);
    }
}
