package dev.zberson.definition;


public interface SQLStrings {
    String ALL_REQUESTS = "SELECT\r\n" + 
            "    r.status_id as Status,\r\n" + 
            "    r.reason_id as Reason,\r\n" + 
            "    r.amount as Amount,\r\n" + 
            "    e.empl_id as Employee_ID,\r\n" + 
            "    e.full_name as Employee_Name,\r\n" + 
            "    e.user_name as Employee_Username,\r\n" + 
            "    TO_CHAR(r.time_submitted, 'MM-DD-YYYY HH24:MI:SS') as Submit_Time,\r\n" + 
            "    TO_CHAR(r.time_processed, 'MM-DD-YYYY HH24:MI:SS') as Process_Time,\r\n" + 
            "    r.note as Note,\r\n" +
            "    r.picture as Pic\r\n" +
            "FROM reimbursements r \r\n" + 
            "INNER JOIN employees e\r\n" + 
            "    ON e.empl_id = r.empl_id_submit \r\n" + 
            "WHERE \r\n" + 
            "    status_id = ?";

    String ALL_REQUESTS_ANY = "SELECT\r\n" + 
            "    r.status_id as Status,\r\n" + 
            "    r.reason_id as Reason,\r\n" + 
            "    r.amount as Amount,\r\n" + 
            "    e.empl_id as Employee_ID,\r\n" + 
            "    e.full_name as Employee_Name,\r\n" + 
            "    e.user_name as Employee_Username,\r\n" + 
            "    TO_CHAR(r.time_submitted, 'MM-DD-YYYY HH24:MI:SS') as Submit_Time,\r\n" + 
            "    TO_CHAR(r.time_processed, 'MM-DD-YYYY HH24:MI:SS') as Process_Time,\r\n" + 
            "    r.note as Note,\r\n" +
            "    r.picture as Pic\r\n" +
            "FROM reimbursements r \r\n" + 
            "INNER JOIN employees e\r\n" + 
            "    ON e.empl_id = r.empl_id_submit \r\n";
    
    String USER_REQUESTS = "SELECT\r\n" + 
            "    r.status_id as Status,\r\n" + 
            "    r.reason_id as Reason,\r\n" + 
            "    r.amount as Amount,\r\n" + 
            "    e.empl_id as Employee_ID,\r\n" + 
            "    e.full_name as Employee_Name,\r\n" + 
            "    e.user_name as Employee_Username,\r\n" + 
            "    TO_CHAR(r.time_submitted, 'MM-DD-YYYY HH24:MI:SS') as Submit_Time,\r\n" + 
            "    TO_CHAR(r.time_processed, 'MM-DD-YYYY HH24:MI:SS') as Process_Time,\r\n" + 
            "    r.note as Note,\r\n" +
            "    r.picture as Pic\r\n" +
            "FROM reimbursements r \r\n" + 
            "INNER JOIN employees e\r\n" + 
            "    ON e.empl_id = r.empl_id_submit \r\n" + 
            "WHERE \r\n" + 
            "    status_id = ?    AND \r\n" + 
            "    e.user_name = ?";
    
    String USER_REQUESTS_ANY = "SELECT\r\n" + 
            "    r.status_id as Status,\r\n" + 
            "    r.reason_id as Reason,\r\n" + 
            "    r.amount as Amount,\r\n" + 
            "    e.empl_id as Employee_ID,\r\n" + 
            "    e.full_name as Employee_Name,\r\n" + 
            "    e.user_name as Employee_Username,\r\n" + 
            "    TO_CHAR(r.time_submitted, 'MM-DD-YYYY HH24:MI:SS') as Submit_Time,\r\n" + 
            "    TO_CHAR(r.time_processed, 'MM-DD-YYYY HH24:MI:SS') as Process_Time,\r\n" + 
            "    r.note as Note,\r\n" +
            "    r.picture as Pic\r\n" +
            "FROM reimbursements r \r\n" + 
            "INNER JOIN employees e\r\n" + 
            "    ON e.empl_id = r.empl_id_submit \r\n" + 
            "WHERE \r\n" +  
            "    e.user_name = ?";
    
    String SUBMIT_REQUEST = "INSERT\r\n" + 
            "    INTO reimbursements(\r\n" + 
            "        status_id,\r\n" + 
            "        reason_id,\r\n" + 
            "        amount,\r\n" + 
            "        empl_id_submit,\r\n" + 
            "        note)\r\n" + 
            "    VALUES (\r\n" + 
            "        ?, ?, ?, (SELECT e.empl_id from employees e where e.user_name = ?), ?\r\n" + 
            "    )";
    
    String SUBMIT_REQUEST_PIC = "INSERT\r\n" + 
            "    INTO reimbursements(\r\n" + 
            "        status_id,\r\n" + 
            "        reason_id,\r\n" + 
            "        amount,\r\n" + 
            "        empl_id_submit,\r\n" + 
            "        note,\r\n" +
            "        picture)\r\n" + 
            "    VALUES (\r\n" + 
            "        ?, ?, ?, (SELECT e.empl_id from employees e where e.user_name = ?), ?, ?\r\n" + 
            "    )";
    
    String PROCESS_REQUEST = "UPDATE reimbursements\r\n" + 
            "    SET status_id = ?,\r\n" + 
            "        time_processed = SYSDATE\r\n" + 
            "    WHERE\r\n" + 
            "        empl_id_submit = (SELECT empl_id FROM employees WHERE user_name = ?) AND\r\n" + 
            "        time_submitted = TO_DATE(?, 'MM-DD-YYYY HH24:MI:SS') AND\r\n" +
            "        status_id = 1";
    
    String CALL_GET_PASSHASH = "{? = call GET_PASSHASH(?,?)}";
    
    String GET_EMPLOYEE = "SELECT * FROM employees\r\n" + 
              "    WHERE user_name = ? AND"
            + "          pass_hash = ?";
}
