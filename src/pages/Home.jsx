import ImgIcon from "../components/icons/ImgIcon";
import bookIcon from "../assets/book_1.svg";
import idCardIcon from "../assets/id_card.svg"
import busIcon from "../assets/bus.svg"
import personEditIcon from "../assets/person_edit.svg"
import { Link } from "react-router-dom";

function Home(){
  return(
    <div className="text-neutral-50 px-40">
      <nav className="flex flex-col gap-10 py-20 ">
        <Link className="text-3xl underline" to="/em-progresso">
          <ImgIcon width={80} src={bookIcon} text={"LIVRO PONTO"}/>
        </Link>
        <Link className="text-3xl underline" to="/em-progresso">
          <ImgIcon width={80} src={personEditIcon} text={"FICHA DO ALUNO"}/>
        </Link>
        <Link className="text-3xl underline" to="/em-progresso">
          <ImgIcon width={80}  src={busIcon} text={"REQUERIMENTO DE TRANSPORTE ESCOLAR"}/>
        </Link>
        <Link className="text-3xl underline" to="/em-progresso">
          <ImgIcon width={80}  src={busIcon} text={"REQUERIMENTO DE TRANSPORTE JORNADA AMPLIADA"}/>
        </Link>
        <Link className="text-3xl underline" to="/cracha">
          <ImgIcon width={80} src={idCardIcon} text={"CRACHÁS"} />
        </Link>
      </nav>
    </div>
  );
}

export default Home;
