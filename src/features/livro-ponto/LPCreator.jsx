import { useEffect, useRef } from "react";
import { useNavigate } from "react-router-dom";
import CreateBookDialog from "./components/dialogs/CreateBookDialog";
import { umes } from "./umes";
import { useLocalModal } from "../../hooks/useLocalModal";
import { useLPBookActions, useLPBookList } from "./stores/LPBookStore";
import { useConfDialogActions } from "../../stores/confDialogStore";

function LPCreator(){
  const bookList = useLPBookList();
  const { updateList, addBook, delBook } = useLPBookActions();
  const {openConfDialog} = useConfDialogActions();
  const [createBookFlag, openCreateBook, closeCreateBook] = useLocalModal();
  const navigate = useNavigate(); 
  const yearInputRef = useRef(null);

  //debug
  //console.log("From Component");

  const handleCreateBook = ({bookName, refMonth, useLastFalg})=>{
    addBook({unidade: 'UME-MC', type: 'ADM', name:bookName});

    //debug
    //console.log(bookList);
  };

  useEffect(()=>{
    yearInputRef.current.value = new Date().getFullYear();
    updateList();
  // eslint-disable-next-line react-hooks/exhaustive-deps
  },[])

  return(
    <div className="flex flex-col grow min-h-0 px-20">

      <div className="py-4 flex items-center gap-4 text-neutral-50">
        <label>Unidade Administrativa / Escolar:
          <select className="text-black ml-1">
            {umes.map((_, idx)=>{
              return (
                <option key={idx}>{umes[idx].toUpperCase()}</option>
              );
            })}
          </select>
        </label>

        <label>
          Tipo:
          <select className="text-black ml-1">
            <option>Professores</option>
            <option>Funcionários</option>
            <option>Equipe</option>
          </select>
        </label>

        <label>
          Ano Base:
          <input ref={yearInputRef} className="text-black w-20 ml-1" type="number"/>
        </label>

      </div>

      <div className="bg-blue-100 grow flex flex-col overflow-y-scroll">
        {bookList.map((book,idx)=>{
            const handleDelBook = ()=>{
              openConfDialog({
                text: "Tem certeza que quer excluir este Livro Ponto?",
                onConfirm: () => delBook(book.id),
              });
            }
          return (
            <div key={idx} className={"flex py-1 justify-between cursor-pointer odd:bg-neutral-50 even:bg-neutral-300 hover:bg-blue-300"}>
              <span className="text-2xl font-semibold">{book.name}</span>
              <div className="flex gap-1">
                <button className="primary-btn" onClick={() => {navigate('/livro-ponto/editor')}}>Editar</button>
                <button className="primary-btn" onClick={handleDelBook}>Excluir</button>
              </div>
            </div>
          );
        })}
      </div>

      <div className="py-4 flex items-center gap-2">
        <button className="sucess-btn ml-auto" onClick={openCreateBook}>Inserir Novo</button>
      </div>

      <CreateBookDialog open={createBookFlag} onClose={closeCreateBook} onConfirm={handleCreateBook}/>
    </div>
  );
}

export default LPCreator;