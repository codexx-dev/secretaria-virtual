import { useState } from "react";
import PaperPage from "../../components/misc/PaperPage"
import LPAdmPage from "./misc/LPAdmPage"
import LPIndice from "./misc/LPIdice";
import LPCapa from "./misc/LPCapa";
import LPAbertura from "./misc/LPAbertura";
import LPFecho from "./misc/LPFecho";

function LPEditor(){
  const [numPage, setNumPage] = useState(1);
  const maxPage = 10;

  const nextPage = ()=>{
    setNumPage(Math.min(numPage + 1, maxPage));
  };
  const prevPage = ()=>{
    setNumPage(Math.max(numPage - 1, 1));
  };
  const gotoPage = (numPage)=>{
    setNumPage(Math.max(Math.min(numPage, maxPage), 1));
  };
  
  const printCalback = ()=>{
    alert("ok");
  };

  return(
    <div className="flex flex-col grow min-h-0">
      
      <div className="header px-3 py-4 flex gap-4 text-neutral-50">
        <div className="flex gap-1">
          <button className="primary-btn">Editar Página</button>
          <button className="primary-btn" onClick={printCalback}>Imprimir Página</button>
          <button className="primary-btn" onClick={printCalback}>Imprimir Tudo</button>
        </div>
        
        <div className="flex gap-1 ml-auto">
          <button className="sucess-btn">Inserir Página</button>
          <button className="cancel-btn">Remover Página</button>
        </div>
      </div>

      <div className="paper-waper *:scale-125 min-h-0 grow overflow-y-scroll bg-neutral-400 p-50 flex gap-70">
        <PaperPage>
          <LPCapa/>
        </PaperPage>

        <PaperPage>
          <LPAbertura/>
        </PaperPage>
        
        <PaperPage>
          <LPIndice/>
        </PaperPage>

        <PaperPage>
          <LPAdmPage/>
        </PaperPage>

        <PaperPage>
          <LPFecho/>
        </PaperPage>
      </div>
      
      {/*<Modal title={"HELLO"}>Hello World</Modal>*/}

      <div className="footer px-3 py-4 flex items-center gap-2">
        <button className="primary-btn">Exportar</button>
        <button className="primary-btn">Importar</button>

        <span className="ml-auto text-center font-semibold text-neutral-50">{`Página: ${numPage}/${maxPage}`}</span>
        <button className="primary-btn font-extrabold" onClick={prevPage}>{"<"}</button>
        <button className="primary-btn font-extrabold" onClick={nextPage}>{">"}</button>
        <button className="primary-btn font-extrabold" onClick={()=>{gotoPage(1)}}>{"<<"}</button>
        <button className="primary-btn font-extrabold" onClick={()=>{gotoPage(maxPage)}}>{">>"}</button>
        <button className="primary-btn" onClick={printCalback}>Ir para...</button>
      </div>
    </div>
  );
}

export default LPEditor;