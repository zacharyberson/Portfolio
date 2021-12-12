package dev.zberson.dao;

import java.sql.Connection;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.SQLIntegrityConstraintViolationException;
import java.sql.SQLTimeoutException;
import java.sql.Types;
import java.util.ArrayList;
import java.util.List;

import dev.zberson.exception.CustomException;
import dev.zberson.exception.CustomException.RequestAlreadyProcessedException;
import dev.zberson.model.Request;
import dev.zberson.util.ConnectionUtil;
import dev.zberson.definition.SQLStrings;


public class RequestDAOImpl implements RequestDAO {

    public List<Request> getRequests(Request r) {
        ArrayList<Request> list = new ArrayList<Request>();
        Connection conn = ConnectionUtil.getConnection();
        String sql = null;
        PreparedStatement statement = null;
        ResultSet rs = null;
        Request row = null;
        int status = r.getStatus();
        int nameSpot = 0;
        
        if(null != r.getUsername()) {
            if(0 == status) { 
                nameSpot = 1;
                sql = SQLStrings.USER_REQUESTS_ANY;
            }
            else {
                nameSpot = 2;
                sql = SQLStrings.USER_REQUESTS;
            }
        }
        else {
            if(0 == status)
                sql = SQLStrings.ALL_REQUESTS_ANY;
            else
                sql = SQLStrings.ALL_REQUESTS;                
        }
        
        try {
            statement = conn.prepareStatement(sql);
            if(0 != status)
                statement.setInt(1, r.getStatus());
            if(null != r.getUsername())
                statement.setString(nameSpot, r.getUsername());
            if(statement.execute()) {
                rs = statement.getResultSet();
                while(rs.next()) {
                    row = new Request(
                        rs.getInt("Status"),
                        rs.getInt("Reason"),
                        rs.getDouble("Amount"),
                        rs.getInt("Employee_ID"),
                        rs.getString("Employee_Username"),
                        rs.getString("Employee_Name"),
                        rs.getString("Submit_Time"),
                        rs.getString("Process_Time"),
                        rs.getString("Note"),
                        rs.getString("Pic")
                    );
                    list.add(row);
                }
                return list;
            }
            else
                return null;
        } catch (SQLTimeoutException s) {
            CustomException.printe("SQL timeout");
        } catch (SQLException e) {
            e.printStackTrace();
        } finally {
            ConnectionUtil.closeConnection();
        }
        return null;
    }

    public void submitRequest(Request r)
            throws SQLIntegrityConstraintViolationException,
                SQLTimeoutException, SQLException {
        
        Connection conn = ConnectionUtil.getConnection();
        String sql = null;
        PreparedStatement statement = null;

        if(null != r.getImg())
            sql = SQLStrings.SUBMIT_REQUEST_PIC;
        else
            sql = SQLStrings.SUBMIT_REQUEST;
        statement = conn.prepareStatement(sql);
        statement.setInt(1, r.getStatus());
        statement.setInt(2, r.getReason());
        statement.setDouble(3, r.getAmount());
        statement.setString(4, r.getUsername());
        if(null != r.getNote())
            statement.setString(5, r.getNote());
        else
            statement.setNull(5, Types.VARCHAR);
        if(null != r.getImg())
            statement.setString(6, r.getImg());
        
        statement.execute();
    }

    public void processRequest(Request r)
            throws SQLIntegrityConstraintViolationException,
            SQLTimeoutException, SQLException,
            RequestAlreadyProcessedException {
        
        Connection conn = ConnectionUtil.getConnection();
        String sql = SQLStrings.PROCESS_REQUEST;
        PreparedStatement statement = null;
    
        statement = conn.prepareStatement(sql);
        statement.setInt(1, r.getStatus());
        statement.setString(2, r.getUsername());
        statement.setString(3, r.getSubmitted());
        if(0 == statement.executeUpdate())
            throw new CustomException.RequestAlreadyProcessedException();
    }
}
