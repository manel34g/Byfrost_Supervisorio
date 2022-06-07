var btn_sidebar = new Boolean(false);
function showSidebar(){
	btn_sidebar= !btn_sidebar;
	var Sidebar = document.getElementById("sidebar");
	var Menu = document.getElementById("menu");
	if(btn_sidebar==1){
		Sidebar.style.marginLeft="0";
		Menu.style.width="70vw"
	}else{
		Sidebar.style.marginLeft="-30vw";
		Menu.style.width="100vw";
	}
}