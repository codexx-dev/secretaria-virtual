import ImgIcon from "../components/icons/ImgIcon";
import bookIcon from "../assets/book_1.svg";
import idCardIcon from "../assets/id_card.svg"
import busIcon from "../assets/bus.svg"
import personEditIcon from "../assets/person_edit.svg"

function Home(){
  return(
    <div className="text-neutral-50 px-40">
      <nav className="flex flex-col gap-10 py-20 ">
        <a className="text-3xl underline" href="./livro-ponto">
          <ImgIcon width={80} src={bookIcon} text={"LIVRO PONTO"}/>
        </a>
        <a className="text-3xl underline" href="./em-progresso">
          <ImgIcon width={80} src={personEditIcon} text={"FICHA DO ALUNO"}/>
        </a>
        <a className="text-3xl underline" href="./em-progresso">
                    <ImgIcon width={80}  src={busIcon} text={"REQUERIMENTO DE TRANSPORTE ESCOLAR"}/>
        </a>
        <a className="text-3xl underline" href="./em-progresso">
          <ImgIcon width={80}  src={busIcon} text={"REQUERIMENTO DE TRANSPORTE JORNADA AMPLIADA"}/>
        </a>
        <a className="text-3xl underline" href="./em-progresso">
          <ImgIcon width={80} src={idCardIcon} text={"CRACHÁS"} />
        </a>
      </nav>
    </div>
  );
}

export default Home;
