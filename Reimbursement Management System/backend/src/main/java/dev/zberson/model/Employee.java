package dev.zberson.model;

import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;

import dev.zberson.exception.CustomException;

public class Employee {
	private String fullName = null;
	private String username = null;
	private String passHash = null;
	private boolean isManager = false;

    private static final int MAX_USERNAME_LENGTH = 12;
    private static final int MAX_PASSWORD_LENGTH = 32;
	
	public Employee() {
		super();
	}
	
	public Employee(String fullName, String username, boolean isManager) {
		this(fullName, username, null, isManager);
	}
	
    public Employee(String fullName, String username, String password, 
            String isManager) {
        this(fullName, username, password, "1".equals(isManager));        
    }

    public Employee(String fullName, String username, String password, 
            boolean isManager) {
        super();
        this.fullName = fullName;
        setUsername(username);
        if(null != password)
            this.passHash = hashPassword(password);
        this.isManager = isManager;
    }
	
	public String getFullName() {
		return fullName;
	}
	
	public void setFullName(String fullName) {
		this.fullName = fullName;
	}
	
	public String getUsername() {
		return username;
	}
	
	public void setUsername(String username) {
	    int max = username.length() < MAX_USERNAME_LENGTH ? username.length() : MAX_USERNAME_LENGTH;
		this.username = new String(username.toCharArray(),0,max);
	}
	
	public String getPassHash() {
		return passHash;
	}
	
	public void setPassHash(String password) {
		this.passHash = hashPassword(password);
	}
	
	public boolean isManager() {
		return isManager;
	}
	
	public void setManager(boolean isManager) {
		this.isManager = isManager;
	}
	
	public void setPreHashedPass(String preHashedPass) {
	    this.passHash = preHashedPass;
	}
	
	private String hashPassword(String password) {
        MessageDigest digest = null;
        byte[] b = null;
        int max = password.length() < MAX_PASSWORD_LENGTH ? password.length() : MAX_PASSWORD_LENGTH;
        
        try{
            digest = MessageDigest.getInstance("SHA-256");
        } catch(NoSuchAlgorithmException e) {
            e.printStackTrace();
            CustomException.printe("incorrect algorithm string");
            return null;
        }
        
        b = digest.digest(password.substring(0,max).getBytes());
        return byteToHex(b);
    }
	
    private String byteToHex(byte[] barray) {
        StringBuilder builder = new StringBuilder();
        
        for(byte b : barray){
            builder.append(String.format("%02X", b));
        }
        return builder.toString();
    }
}
