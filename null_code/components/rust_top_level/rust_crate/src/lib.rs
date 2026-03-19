#![cfg_attr(not(feature = "std"), no_std)]

use core::ffi::c_void; 
use esp_idf_sys::esp_restart; 

#[panic_handler]
fn panic(info: &PanicInfo) -> ! {
    unsafe {
        esp_restart();

    }

}




