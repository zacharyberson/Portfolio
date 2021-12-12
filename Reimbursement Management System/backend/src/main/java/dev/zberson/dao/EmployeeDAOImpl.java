package dev.zberson.dao;

import java.sql.CallableStatement;
import java.sql.Connection;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.SQLTimeoutException;
import java.sql.Types;

import dev.zberson.definition.SQLStrings;
import dev.zberson.model.Employee;
import dev.zberson.util.ConnectionUtil;

public class EmployeeDAOImpl implements EmployeeDAO{
    

    public Employee checkCredentials(Employee employee) throws SQLTimeoutException, SQLException {                       
        Connection conn = ConnectionUtil.getConnection();
        String sql = SQLStrings.GET_EMPLOYEE;
        ResultSet rs = null;
        String newPass = null;

        System.out.println("it is: " + employee.getUsername() + ", " + employee.getPassHash());
        newPass = getPasshash(employee);
        CallableStatement call = conn.prepareCall(sql);
        call.setString(1, employee.getUsername());
        call.setString(2, newPass);
        if(call.execute()) {
            rs = call.getResultSet();
            if(rs.next()) {
                Employee eNew = new Employee(
                        rs.getString(2),
                        rs.getString(3),
                        null,
                        rs.getString(6)
                        );
                eNew.setPreHashedPass(rs.getString(4));
                return eNew;
            }
        }
        call.close();
        return null;
    }
    
    private String getPasshash(Employee e) throws SQLTimeoutException, SQLException {
        Connection conn = ConnectionUtil.getConnection();
        String sql = SQLStrings.CALL_GET_PASSHASH;
        String sResult = null;
        
        CallableStatement call = conn.prepareCall(sql);
        call.registerOutParameter(1, Types.VARCHAR);
        call.setString(2,e.getUsername());
        call.setString(3,e.getPassHash());
        call.execute();
        sResult = call.getString(1);
        if(null != sResult)
            sResult = sResult.toUpperCase();
        call.close();
        return sResult;
    }
}
