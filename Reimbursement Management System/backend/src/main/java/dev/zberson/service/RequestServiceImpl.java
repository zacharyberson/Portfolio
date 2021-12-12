package dev.zberson.service;

import java.sql.SQLException;
import java.sql.SQLIntegrityConstraintViolationException;
import java.sql.SQLTimeoutException;
import java.util.List;

import dev.zberson.model.Request;
import dev.zberson.util.ConnectionUtil;
import dev.zberson.dao.RequestDAO;
import dev.zberson.dao.RequestDAOImpl;
import dev.zberson.definition.Status;
import dev.zberson.exception.CustomException;
import dev.zberson.exception.CustomException.RequestAlreadyProcessedException;

public class RequestServiceImpl implements RequestService {
    
    private RequestDAO dao = null;
    
    public RequestServiceImpl() {
        dao = new RequestDAOImpl();
    }

	public List<Request> getRequests(int status, String username) {
		return dao.getRequests(new Request(status, username));
	}

    public boolean submitRequest(int reason, double amount,
            String username, String note) {
        
        try {
            dao.submitRequest(new Request(1,reason,amount,username,note));
            return true;
        } catch (SQLIntegrityConstraintViolationException s) {
            CustomException.printe("request already submitted");
        } catch (SQLTimeoutException s) {
            CustomException.printe("SQL timeout");
        } catch (SQLException e) {
            e.printStackTrace();
        } finally {
            ConnectionUtil.closeConnection();
        }
    
        return false;
    }

    public boolean submitRequest(int reason, double amount,
            String username, String note, String img) {
        Request r = new Request(1,reason,amount,username,note);
        r.addImg(img);
        
        try {
            dao.submitRequest(r);
            return true;
        } catch (SQLIntegrityConstraintViolationException s) {
            CustomException.printe("request already submitted");
        } catch (SQLTimeoutException s) {
            CustomException.printe("SQL timeout");
        } catch (SQLException e) {
            e.printStackTrace();
        } finally {
            ConnectionUtil.closeConnection();
        }
    
        return false;
    }

	public boolean approveRequest(String username, String timeSubmitted) {
	    try {
    		dao.processRequest(new Request(Status.APPROVED, username, timeSubmitted));
            return true;
        } catch (SQLIntegrityConstraintViolationException s) {
            CustomException.printe("request already processed");
        } catch (SQLTimeoutException s) {
            CustomException.printe("SQL timeout");
        } catch (SQLException e) {
            e.printStackTrace();
        } catch (RequestAlreadyProcessedException e) {
            System.out.println("request already processed");
        } finally {
            ConnectionUtil.closeConnection();
        }
        return false;
	}

    public boolean denyRequest(String username, String timeSubmitted) {
        try {
            dao.processRequest(new Request(Status.DENIED, username, timeSubmitted));
            return true;
        } catch (SQLIntegrityConstraintViolationException s) {
            CustomException.printe("request already processed");
        } catch (SQLTimeoutException s) {
            CustomException.printe("SQL timeout");
        } catch (SQLException e) {
            e.printStackTrace();
        } catch (RequestAlreadyProcessedException e) {
            System.out.println("request already processed");
        } finally {
            ConnectionUtil.closeConnection();
        }
        return false;
    }
}
