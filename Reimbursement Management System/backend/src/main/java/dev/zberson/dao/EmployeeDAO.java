package dev.zberson.dao;

import java.sql.SQLException;
import java.sql.SQLTimeoutException;

import dev.zberson.model.Employee;

public interface EmployeeDAO {
    
    /*
     * check r.username and r.passHash for validity
     * 
     * Return: Employee of matching employee with full details
     *          null if password incorrect or no such username
     */
    public Employee checkCredentials(Employee employee)
            throws SQLTimeoutException, SQLException;
}