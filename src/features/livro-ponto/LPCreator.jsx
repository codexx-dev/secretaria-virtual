import { useEffect, useRef, useState } from "react";
import CreateBookDialog from "./dialogs/CreateBookDialog";
import { umes } from "./umes";
import EntryDialog from "../../components/dialogs/EntryDialog";
import CreateUMEDialog from "./dialogs/CreateUMEDialog";
import { useNavigate } from "react-router-dom";
import LPEditor from "./LPEditor";

function LPCreator(){
  //const [selectedBook, setSelectedBook] = useState(-1);
  const [dialogFlags, setDialogFlags] = useState({
    createBook: false,
    addUnidade: false,
    deleteBook: false,
    deleteUnidade: false
  });
  const navigate = useNavigate(); 
  const yearInputRef = useRef(null);

  useEffect(()=>{
    yearInputRef.current.value = new Date().getFullYear();
  },[])

  return(
    <div className="flex flex-col grow min-h-0 px-20">

      <div className="py-4 flex items-center gap-2 text-neutral-50">
        <label>Selecionar Unidade:
          <select className="text-black">
            {umes.map((_, idx)=>{
              return (
                <option key={idx}>{umes[idx].toUpperCase()}</option>
              );
            })}
          </select>
        </label>

        <button className="primary-btn" onClick={() => {setDialogFlags({...dialogFlags, addUnidade: true});}}>Adicionar</button>
        <button className="primary-btn" onClick={() => {setDialogFlags({...dialogFlags, deleteUnidade: true});}} >Remover</button>

        <label className="ml-5">
          Ano Base:
          <input ref={yearInputRef} className="text-black w-20 ml-1" type="number"/>
        </label>

        <label>
          Tipo:
          <select className="text-black ml-1">
            <option>Professores</option>
            <option>Funcionários</option>
            <option>Equipe</option>
          </select>
        </label>

      </div>

      <div className="bg-blue-100 grow flex flex-col overflow-y-scroll">
        {Array.from({length: 15}).map((_,idx)=>{
          return (
            <div key={idx} className={"flex py-1 justify-between cursor-pointer odd:bg-neutral-50 even:bg-neutral-300 hover:bg-blue-300"}>
              <span className="text-2xl font-semibold">UME Rural Monte Cabrão</span>
              <div className="flex gap-1">
                <button className="primary-btn" onClick={() => {navigate('/livro-ponto-editor')}}>Editar</button>
                <button className="primary-btn" onClick={() => {setDialogFlags({...dialogFlags, deleteBook: true});}}>Excluir</button>
              </div>
            </div>
          );
        })}
      </div>

      <div className="py-4 flex items-center gap-2">
        <button className="sucess-btn font-extrabold ml-auto" onClick={() => {setDialogFlags({...dialogFlags, createBook: true});}}>Inserir Novo</button>
      </div>

      {/* Dialogs */}

      <CreateUMEDialog open={dialogFlags.addUnidade} onClose={() => {setDialogFlags({...dialogFlags,    addUnidade: false});}}/>
      <EntryDialog open={dialogFlags.deleteUnidade}  onClose={() => {setDialogFlags({...dialogFlags, deleteUnidade: false});}}>
        <p className="text-center text-black text-lg">Tem certeza que deseja excluir esta unidade?</p>
      </EntryDialog>

      <CreateBookDialog open={dialogFlags.createBook} onClose={() => {setDialogFlags({...dialogFlags, createBook: false});}}/>
      <EntryDialog open={dialogFlags.deleteBook}  onClose={() => {setDialogFlags({...dialogFlags, deleteBook: false});}}>
        <p className="text-center text-lg">Tem certeza que deseja excluir este Livro?</p>
      </EntryDialog>
    </div>
  );
}

export default LPCreator;