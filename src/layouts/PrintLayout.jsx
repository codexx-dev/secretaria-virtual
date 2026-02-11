import { Outlet } from "react-router-dom";

function PrintLayout(){
  return(
    <div className="size-full flex flex-col bg-slate-800">
      <Outlet/>
    </div>
  );
}

export default PrintLayout;