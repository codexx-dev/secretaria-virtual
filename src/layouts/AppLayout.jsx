import { Link, Outlet } from "react-router-dom";
import homeIcon from "../assets/home.svg";
import infoIcon from "../assets/info.svg";
import ImgIcon from "../components/icons/ImgIcon";
import MessageDialog from "../components/dialogs/MessageDialog";
import ConfDialog from "../components/dialogs/ConfDialog";


function AppLayout(){
  return(
    <div className="size-full flex flex-col bg-slate-800">
      <div className="no-print px-5 bg-slate-700 text-neutral-50 flex flex-col">

        <div className="flex flex-row justify-center items-center gap-3">
          <h2 className="py-5 ml-2 text-4xl font-bold text-center">SECRETARIA VIRTUAL{/*routeName*/}</h2>
        </div>
        
        <nav className="text-lg underline flex flex-row gap-5">
          <Link className="ml-auto" to="/">
            <ImgIcon src={homeIcon} text={"início"}/>
          </Link>
          <Link to="/sobre/">
            <ImgIcon src={infoIcon} text={"sobre..."}/>
          </Link>
        </nav>
      </div>
      
      <Outlet/>

      <ConfDialog/>
      <MessageDialog/>
    </div>
  );
}

export default AppLayout;