package dev.zberson.util;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.SQLException;
import java.util.Properties;

public class ConnectionUtil {
	private static Connection conn = null;

	public static void main(String[] args) {
		Connection conn = getConnection();
		System.out.println(conn);
	}

	public static Connection getConnection() {
		// If we already have an open connection, return that instead
		// of creating a new one
		try {
			if(conn != null && !conn.isClosed()) {
				return conn;
			}
		} catch (SQLException e) {
			;
		}
		
		// create a new connection and return it
		try {
			Class.forName("oracle.jdbc.driver.OracleDriver");

			// load in properties file values
			//   endpoint
			//   username
			//   password
			Properties props = new Properties();
			File f = new File("connection.properties");
            if(!f.exists()) {
                f = new File("src\\main\\resources\\connection.properties");
          } if(!f.exists()) {
                f = new File("properties\\connection.properties");
          } if(!f.exists()) {
                f = new File("C:\\Users\\ZEEBE\\Dropbox\\Workspace\\Eclipse\\Project1\\src\\main\\resources\\connection.properties") ;
          }
            
			FileInputStream in = new FileInputStream(f);
			props.load(in);

			String endpoint = props.getProperty("endpoint");
			String username = props.getProperty("username");
			String password = props.getProperty("password");

			// pass username and password to endpoint
			// endpoint will reject or accept and return established connection
			conn = DriverManager.getConnection(endpoint, username, password);
			
			
		} catch (FileNotFoundException e) {
			e.printStackTrace();
			return null;
		} catch (IOException e) {
			e.printStackTrace();
			return null;
		} catch (ClassNotFoundException e) {
			e.printStackTrace();
			return null;
		} catch (SQLException e) {
			e.printStackTrace();
			return null;
		}
		
		return conn;
	}

	public static void closeConnection() {
		try {
			if(conn != null)    conn.close();
		} catch (SQLException e) {
			;
		}
		conn = null;
	}
}
