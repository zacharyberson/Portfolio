package dev.zberson.service;

import java.util.List;

import dev.zberson.model.Request;

public interface RequestService {
	
	/*
	 * Get requests of status, if username isn't null then only
	 *     fetch those submitted by username
	 * 
	 * Return: List (possibly empty) of Requests of status Status submitted
	 *         If username not null, then only returns those submitted by username
	 * 			Null if query failed.
	 */
	public List<Request> getRequests(int status, String username);
	

    /*
     * submits Request r to database.
     * Fields required: reason, amount, username
     * Optional: note
     * 
     * Return: return true if Request successfully submitted
     *          false otherwise
     */
    public boolean submitRequest(int reason, double amount,
            String username, String note);

    /*
     * submits Request r to database, with image img
     * Fields required: reason, amount, username, img
     * Optional: note
     * 
     * Return: return true if Request successfully submitted
     *          false otherwise
     */
    public boolean submitRequest(int reason, double amount,
            String username, String note, String img);
	
	/*
	 * approve request indicated by timeSubmitted and username
	 * 
	 * Return: returns true if query successfully updated
	 * 			false otherwise
	 */
	public boolean approveRequest(String username, String timeSubmitted);
	
	
	/*
	 *  deny request indicated by timeSubmitted and username
	 *  
	 *  Return: returns true if query successfully updated, false otherwise
	 */
	public boolean denyRequest(String username, String timeSubmitted);
}
