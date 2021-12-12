package dev.zberson.service;

import dev.zberson.model.Employee;

public interface EmployeeService {
    /*
     * Checks username and password combination for validity
     * 
     * Return: returns Employee info from database if valid username and pass
     *          null otherwise
     */
    public Employee checkCredentials(String username, String password);
}
