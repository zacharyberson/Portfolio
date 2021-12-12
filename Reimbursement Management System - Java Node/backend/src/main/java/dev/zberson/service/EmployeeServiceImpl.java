package dev.zberson.service;

import java.sql.SQLException;
import java.sql.SQLTimeoutException;

import dev.zberson.dao.EmployeeDAO;
import dev.zberson.dao.EmployeeDAOImpl;
import dev.zberson.exception.CustomException;
import dev.zberson.model.Employee;
import dev.zberson.util.ConnectionUtil;

public class EmployeeServiceImpl implements EmployeeService {
    
    EmployeeDAO dao = null;
    
    public EmployeeServiceImpl() {
        super();
        dao = new EmployeeDAOImpl();
    }
    
    public Employee checkCredentials(String username, String password) {
        Employee e = new Employee(null, username, password, false);
        
        try {
            e = dao.checkCredentials(e);
            if(null != e)
                return e;
        } catch (SQLTimeoutException s) {
            CustomException.printe("connection timeout");
        } catch (SQLException s) {
            s.printStackTrace();
        } finally {
            ConnectionUtil.closeConnection();
        }
        return null;
    }
}
