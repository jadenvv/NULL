#![cfg_attr(not(feature = "std"), no_std)]

use core::ffi::c_void; 


static HELLO_ESP: &'static [u8] = b"HELLO " 

#[no_mangle]
pub extern "C" fn hello() -> *const c_void{
    HELLO_ESP.as_ptr() as *const v_void 
}
