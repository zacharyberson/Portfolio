package dev.zberson.dao;

import java.sql.SQLException;
import java.sql.SQLIntegrityConstraintViolationException;
import java.sql.SQLTimeoutException;
import java.util.List;

import dev.zberson.exception.CustomException.RequestAlreadyProcessedException;
import dev.zberson.model.Request;

public interface RequestDAO {   

    /*
     * get requests of r.status submitted by r.username, or all users if
     *  r.username is null
     * 
     * Return: List of Requests of status Status submitted by username
     *          Empty list if no requests or username not registered
     */
    public List<Request> getRequests(Request r);
    
    
    /*
     * submits Request r to database.
     * Fields required: status, reason, amount, username
     * Optional: note (may be null)
     * 
     * Return: return if Request successfully submitted
     *          throws exception otherwise
     */
    public void submitRequest(Request r)
            throws SQLIntegrityConstraintViolationException,
            SQLTimeoutException, SQLException;
    
    
    /*
     * approve/deny using r.status request indicated by r.timeSubmitted,
     *  and r.username
     * 
     * Return: returns true if query successfully updated
     *          false otherwise
     */
    public void processRequest(Request r)
            throws SQLIntegrityConstraintViolationException,
            SQLTimeoutException, SQLException,
            RequestAlreadyProcessedException;
}
