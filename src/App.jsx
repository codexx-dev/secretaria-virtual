import { Routes, Route, useLocation, Link } from "react-router-dom";
import Home from "./pages/Home";
import About from "./pages/About";
import LPCreator from "./features/livro-ponto/LPCreator";
import WorkInProgress from "./pages/WorkInProgress";

// import backArrowIcon from "./assets/arrow_back.svg";
// import forwardArrowIcon from "./assets/arrow_forward.svg";
import homeIcon from "./assets/home.svg";
import infoIcon from "./assets/info.svg";
import ImgIcon from "./components/icons/ImgIcon";
import LPEditor from "./features/livro-ponto/LPEditor";
import Page404 from "./pages/Page404";

function App() {
  const location = useLocation();
  var routeName = location.pathname.substring(1).toUpperCase().replaceAll('-', ' ');
  if(routeName) routeName = ' / ' + routeName;

  return (
    <div className="size-full flex flex-col bg-slate-800">
      <div className="header px-5 bg-slate-700 text-neutral-50 flex flex-col">

        <div className="flex flex-row justify-center items-center gap-3">
          {/* <button className="size-12 px-2 bg-[rgba(0,0,0,0.25)] hover:bg-[rgba(0,0,0,0.30)] active:bg-[rgba(0,0,0,0.40)] rounded-md"
          onClick={()=>{window.history.back();}}>
            <img src={backArrowIcon}/>
          </button>
          <button className="size-12 px-2 bg-[rgba(0,0,0,0.25)] hover:bg-[rgba(0,0,0,0.30)] active:bg-[rgba(0,0,0,0.40)] rounded-md"
          onClick={()=>{window.history.forward();}}>
            <img src={forwardArrowIcon}/>
          </button> */}
          <h2 className="py-5 ml-2 text-4xl font-bold text-center">SECRETARIA VIRTUAL{routeName}</h2>
        </div>
        
        <nav className="text-lg underline flex flex-row gap-5">
          <Link className="ml-auto" to="/">
            <ImgIcon src={homeIcon} text={"início"}/>
          </Link>
          <Link to="/sobre">
            <ImgIcon src={infoIcon} text={"sobre..."}/>
          </Link>
        </nav>
      </div>

      <Routes>
        <Route path="/" element={<Home/>}/>
        <Route path="/sobre" element={<About/>}/>
        <Route path="/livro-ponto" element={<LPCreator/>}/>
        <Route path="/livro-ponto-editor" element={<LPEditor/>}/>
        <Route path="/em-progresso" element={<WorkInProgress/>}/>
        <Route path="*" element={<Page404/>}/>
      </Routes>

    </div>
  )
}

export default App
